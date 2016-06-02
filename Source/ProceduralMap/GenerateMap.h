// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"

#include "GameFramework/Actor.h"
#include "GenerateMap.generated.h"

USTRUCT(BlueprintType)
struct FMapStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		TArray<FVector>	Vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		TArray<int32> Triangles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		TArray<FVector> Normals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		TArray<FVector2D> UV0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		TArray<FProcMeshTangent> Tangent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		FVector2D		MapSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		float			MapSquareScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Struct")
		float			MapHeightScale;
};

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

	/*CONFIG*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapConfig")
		UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = "MapConfig")
		FVector2D			MapSize = FVector2D(10, 10);

	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = "MapConfig")
		float				SquareScale = 100;
	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = "MapConfig")
		float				SquareHeightScale = 10;

	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = "MapConfig")
		float				PerlinRes = 10;

	UPROPERTY(EditAnywhere, BluePrintReadwrite, Category = "MapConfig")
		bool				CalculateNormalAndTangent = true;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "MapConfig")
		TEnumAsByte<ECollisionChannel>	CollisionChannel;
	/*END CONFIG*/

	UPROPERTY(BluePrintReadwrite)
		UProceduralMeshComponent *Mesh;
	
public:
	UPROPERTY(BluePrintReadwrite)
		FMapStruct			_Map;
	UFUNCTION(BluePrintCallable, Category = GenerateMap)
		FMapStruct			GenerateMap(int x, int y);
	UFUNCTION(BluePrintCallable, Category = GenerateMap)
		void				UpdateMap(UPARAM(ref) FMapStruct &map);

protected:
	FMapStruct				setQuad(int x, int y, int index);
	void					SetSquareZWithPosition(FMapStruct *map, FVector2D pos, FVector2D mapSize, FVector2D AB, FVector2D CD);
};
