// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "GenerateMap.h"

#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

#include <PerlinNoise.h>

// Sets default values
AGenerateMap::AGenerateMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));

		Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

FMapStruct AGenerateMap::setQuad(int x, int y, int index)
{
	FMapStruct				map;
	int						squareScale = SquareScale;
	int						heightScale = SquareHeightScale;

	map.Vertices.Add(FVector((squareScale * x), (squareScale * y), 1 * heightScale));
	map.Vertices.Add(FVector((squareScale * x) + squareScale, (squareScale * y), 1 * heightScale));
	map.Vertices.Add(FVector((squareScale * x), (squareScale * y) + squareScale, 1 * heightScale));
	map.Vertices.Add(FVector((squareScale * x) + squareScale, (squareScale * y) + squareScale, 1 * heightScale));

	map.UV0.Add(FVector2D(0, 0));
	map.UV0.Add(FVector2D(1, 0));
	map.UV0.Add(FVector2D(0, 1));
	map.UV0.Add(FVector2D(1, 1));

	UKismetProceduralMeshLibrary::ConvertQuadToTriangles(map.Triangles, index + 1, index, index + 2, index + 3);
	return (map);
}

FMapStruct AGenerateMap::GenerateMap(int x, int y)
{
	FMapStruct	map;
	FMapStruct	mapToAppend;
	int			index;

	index = 0;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++, index += 4)
		{
			mapToAppend = setQuad(j, i, index);
			map.Vertices.Append(mapToAppend.Vertices);
			map.Triangles.Append(mapToAppend.Triangles);
			map.UV0.Append(mapToAppend.UV0);
		}
	}

	if (CalculateNormalAndTangent)
		UKismetProceduralMeshLibrary::CalculateTangentsForMesh(map.Vertices, map.Triangles, map.UV0, map.Normals, map.Tangent);
	return (map);
}

void AGenerateMap::SetSquareZWithPosition(FMapStruct *map, FVector2D pos, FVector2D mapSize, FVector2D AB, FVector2D CD)
{
	int i = ((pos.Y * mapSize.X) + pos.X) * 4;

	if (i >= map->Vertices.Num() || map == NULL)
		return;

	map->Vertices[i++].Z = AB.X;
	map->Vertices[i++].Z = AB.Y;
	map->Vertices[i++].Z = CD.X;
	map->Vertices[i++].Z = CD.Y;
}

// Called when the game starts or when spawned
void AGenerateMap::BeginPlay()
{
	Super::BeginPlay();
	
	_Map = GenerateMap(MapSizeX, MapSizeY);

	PerlinNoise perlin = PerlinNoise(MapSizeX + 1, MapSizeY + 1, PerlinRes);
	perlin.setScale(SquareHeightScale);
	perlin.Generate2DPerlinNoise();
	float **heightmap = perlin.getMap();

	for (int i = 0, k = 0; i < MapSizeY; i++)
	{
		for (int j = 0, l = 0; j < MapSizeX; j++)
		{
			FVector2D AB = FVector2D(heightmap[k][l], heightmap[k][l + 1]);
			FVector2D CD = FVector2D(heightmap[k + 1][l], heightmap[k + 1][l + 1]);
			SetSquareZWithPosition(&_Map, FVector2D(j, i), FVector2D(MapSizeX, MapSizeY), AB, CD);
			l++;
		}
		k++;
	}

	Mesh->CreateMeshSection(0, _Map.Vertices, _Map.Triangles, _Map.Normals, _Map.UV0, TArray<FColor>(), _Map.Tangent, true);
	Mesh->SetMaterial(0, Material);
	Mesh->SetCollisionResponseToChannel(CollisionChannel, ECollisionResponse::ECR_Block);
}

// Called every frame
void AGenerateMap::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

int		GetPos(int x, int y, int quadIndice, int sizeX)
{
	if (x < 0 || y < 0)
		return (-1);
	return ((((y * sizeX) + x) * 4) + quadIndice);
}

void	setBrushVertice(TArray<int32> &lst, int pos)
{
	if (pos < 0)
		return;
	lst.Add(pos);
	UE_LOG(LogTemp, Warning, TEXT("%d"), pos);
}

TArray<int32>	AGenerateMap::SuperBrush(FVector2D Pos, FVector2D BrushSize)
{
	int i, j;

	TArray<int32>	verticesToEdit;

	//Les 4 coins
	setBrushVertice(verticesToEdit, GetPos(Pos.X - 1, Pos.Y - 1, DownRight, MapSizeX));
	setBrushVertice(verticesToEdit, GetPos(Pos.X + BrushSize.X, Pos.Y - 1, DownLeft, MapSizeX));
	setBrushVertice(verticesToEdit, GetPos(Pos.X + BrushSize.X, Pos.Y + BrushSize.Y, UpLeft, MapSizeX));
	setBrushVertice(verticesToEdit, GetPos(Pos.X - 1, Pos.Y + BrushSize.Y, UpRight, MapSizeX));

	for (i = Pos.Y; i < Pos.Y + BrushSize.Y; i++)
	{
		//la "gauche" et la "droite" (x - 1 et x + BrushSizeSizeX)
		setBrushVertice(verticesToEdit, GetPos(Pos.X - 1, i, UpRight, MapSizeX));
		setBrushVertice(verticesToEdit, GetPos(Pos.X - 1, i, DownRight, MapSizeX));
		setBrushVertice(verticesToEdit, GetPos(Pos.X + BrushSize.X, i, UpLeft, MapSizeX));
		setBrushVertice(verticesToEdit, GetPos(Pos.X + BrushSize.X, i, DownLeft, MapSizeX));
		for (j = Pos.X; j < Pos.X + BrushSize.X; j++)
		{
			if (i == Pos.Y)
			{
				//le "haut" et le "bas" (y - 1 et y + BrushSizeSizeY)
				setBrushVertice(verticesToEdit, GetPos(j, Pos.Y - 1, DownLeft, MapSizeX));
				setBrushVertice(verticesToEdit, GetPos(j, Pos.Y - 1, DownRight, MapSizeX));
				setBrushVertice(verticesToEdit, GetPos(j, Pos.Y + BrushSize.Y, UpLeft, MapSizeX));
				setBrushVertice(verticesToEdit, GetPos(j, Pos.Y + BrushSize.Y, UpRight, MapSizeX));
			}
			//le centre du carré
			setBrushVertice(verticesToEdit, GetPos(j, i, UpLeft, MapSizeX));
			setBrushVertice(verticesToEdit, GetPos(j, i, UpRight, MapSizeX));
			setBrushVertice(verticesToEdit, GetPos(j, i, DownLeft, MapSizeX));
			setBrushVertice(verticesToEdit, GetPos(j, i, DownRight, MapSizeX));
		}
	}
	return (verticesToEdit);
}
