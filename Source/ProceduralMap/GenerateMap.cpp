// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "GenerateMap.h"

#include "ProceduralMeshComponent.h"


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
	int						squareScale = 100;
	int						heightScale = 1;

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

// Called when the game starts or when spawned
void AGenerateMap::BeginPlay()
{
	Super::BeginPlay();
	
	AGenerateMap::t_map		mapToAppend;
	int						index;

	index = 0;
	for (int i = 0; i < MapSizeY; i++)
	{
		for (int j = 0; j < MapSizeX; j++, index += 6)
		{
			mapToAppend = SetSquare(j, i, index);

			Map.Vertices.Append(mapToAppend.Vertices);
			Map.Triangles.Append(mapToAppend.Triangles);
			Map.Normals.Append(mapToAppend.Normals);
			Map.UV0.Append(mapToAppend.UV0);
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

