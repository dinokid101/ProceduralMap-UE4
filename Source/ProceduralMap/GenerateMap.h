// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"

#include "GameFramework/Actor.h"
#include "GenerateMap.generated.h"

UCLASS()
class PROCEDURALMAP_API AGenerateMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerateMap();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

public:

	typedef struct			s_map
	{
		TArray<FVector>		Vertices;
		TArray<int32>		Triangles;
		TArray<FVector>		Normals;
		TArray<FVector2D>	UV0;
	}						t_map;

	/*CONFIG*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MapConfig)
		UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = MapConfig)
		int32				MapSizeX;
	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = MapConfig)
		int32				MapSizeY;
	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = MapConfig)
		float				SquareScale = 100;
	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = MapConfig)
		float				SquareHeightScale = 100;
	/*END CONFIG*/

	UProceduralMeshComponent *Mesh;
	t_map					Map;

	TArray<FVector>			SetSquareVertices(FVector A, FVector B, FVector C, FVector D);
	t_map					SetSquare(int x, int y, int index);
	
};
