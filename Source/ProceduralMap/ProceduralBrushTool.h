// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ProceduralBrushTool.generated.h"


UENUM(BlueprintType)
enum EQuadIndice
{
	UpLeft = 0 UMETA(DisplayName = "Up left corner"),
	UpRight = 1 UMETA(DisplayName = "Up right corner"),
	DownLeft = 2 UMETA(DisplayName = "Down left corner"),
	DownRight = 3 UMETA(DisplayName = "Down right corner")
};

/**
 * 
 */
UCLASS(Blueprintable)
class PROCEDURALMAP_API UProceduralBrushTool : public UObject
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "Brush")
		TArray<int32>	SquareBrush(FVector2D pos, FVector2D brushSize, FVector2D mapSize);

	UFUNCTION(BlueprintCallable, Category = "Brush")
		int		GetPos(int x, int y, int quadIndice, int sizeX);

	UFUNCTION(BlueprintCallable, Category = "Brush")
		void	SetVertice(TArray<int32> &lst, int pos);	
};
