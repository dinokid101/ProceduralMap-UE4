// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "GenerateMap.h"
#include "ProceduralMeshComponent.h"

#include <cstdlib>
#include <ctime>

// Sets default values
AGenerateMap::AGenerateMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
		Mesh->AttachTo(RootComponent);
	}
}

TArray<FVector> AGenerateMap::SetSquareVertices(FVector A, FVector B, FVector C, FVector D)
{
	TArray<FVector> square;

	square.Add(FVector(B.X, B.Y, B.Z));
	square.Add(FVector(A.X, A.Y, A.Z));
	square.Add(FVector(C.X, C.Y, C.Z));

	square.Add(FVector(C.X, C.Y, C.Z));
	square.Add(FVector(D.X, D.Y, D.Z));
	square.Add(FVector(B.X, B.Y, B.Z));
	return (square);
}

AGenerateMap::t_map AGenerateMap::SetSquare(int x, int y, int index)
{
	AGenerateMap::t_map		map;
	int						squareScale = SquareScale;
	int						heightScale = SquareHeightScale;

	FVector A = FVector((squareScale * x), (squareScale * y), 1 * heightScale);
	FVector B = FVector((squareScale * x) + squareScale, (squareScale * y), 1 * heightScale);
	FVector C = FVector((squareScale * x), (squareScale * y) + squareScale, 1 * heightScale);
	FVector D = FVector((squareScale * x) + squareScale, (squareScale * y) + squareScale, 1 * heightScale);

	map.Vertices = SetSquareVertices(A, B, C, D);

	map.Triangles.Add(index++);
	map.Triangles.Add(index++);
	map.Triangles.Add(index++);

	map.Triangles.Add(index++);
	map.Triangles.Add(index++);
	map.Triangles.Add(index++);

	map.Normals.Add(FVector(0, -1, 0));
	map.Normals.Add(FVector(0, -1, 0));
	map.Normals.Add(FVector(0, -1, 0));

	map.Normals.Add(FVector(0, -1, 0));
	map.Normals.Add(FVector(0, -1, 0));
	map.Normals.Add(FVector(0, -1, 0));


	map.UV0.Add(FVector2D(1, 0));
	map.UV0.Add(FVector2D(0, 0));
	map.UV0.Add(FVector2D(0, 1));

	map.UV0.Add(FVector2D(0, 1));
	map.UV0.Add(FVector2D(1, 1));
	map.UV0.Add(FVector2D(1, 0));
	return (map);
}

AGenerateMap::t_map AGenerateMap::GenerateMap(int x, int y)
{
	AGenerateMap::t_map		map;
	AGenerateMap::t_map		mapToAppend;
	int						index;

	index = 0;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++, index += 6)
		{
			mapToAppend = SetSquare(j, i, index);

			map.Vertices.Append(mapToAppend.Vertices);
			map.Triangles.Append(mapToAppend.Triangles);
			map.Normals.Append(mapToAppend.Normals);
			map.UV0.Append(mapToAppend.UV0);
		}
	}
	return (map);
}

void AGenerateMap::SetSquareZWithPosition(AGenerateMap::t_map *map, FVector2D pos, FVector2D mapSize, FVector2D AB, FVector2D CD)
{
	int i = ((pos.Y * mapSize.X) + pos.X) * 6;

	if (i >= map->Vertices.Num() || map == NULL)
		return;

	map->Vertices[i++].Z = AB.Y;
	map->Vertices[i++].Z = AB.X;
	map->Vertices[i++].Z = CD.X;

	map->Vertices[i++].Z = CD.X;
	map->Vertices[i++].Z = CD.Y;
	map->Vertices[i++].Z = AB.Y;
}

// Called when the game starts or when spawned
void AGenerateMap::BeginPlay()
{
	Super::BeginPlay();
	
	Map = GenerateMap(MapSizeX, MapSizeY);

	{
		srand(time(NULL));
		int32 heightmap[101][101] = {};

		for (int i = 0; i < MapSizeY + 1; i++)
		{
			for (int j = 0; j < MapSizeX + 1; j++)
			{
				heightmap[i][j] = rand() % 500;
			}
		}

		for (int i = 0, k = 0; i < MapSizeY; i++)
		{
			for (int j = 0, l = 0; j < MapSizeX; j++)
			{
				FVector2D AB = FVector2D(heightmap[k][l], heightmap[k][l + 1]);
				FVector2D CD = FVector2D(heightmap[k + 1][l], heightmap[k + 1][l + 1]);
				SetSquareZWithPosition(&Map, FVector2D(j, i), FVector2D(MapSizeX, MapSizeY), AB, CD);
				l++;
			}
			k++;
		}
	}

	Mesh->CreateMeshSection(0, Map.Vertices, Map.Triangles, Map.Normals, Map.UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	Mesh->SetMaterial(0, Material);
}

// Called every frame
void AGenerateMap::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

