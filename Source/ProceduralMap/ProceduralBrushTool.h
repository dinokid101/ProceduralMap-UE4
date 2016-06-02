// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GenerateMap.h>

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
		void			SquareBrush(FVector2D pos, float ZValue, FVector2D BrushSize, UPARAM(ref) FMapStruct &map);
		UFUNCTION(BlueprintCallable, Category = "Brush")
		void			CircleBrush(FVector2D pos, float ZValue, int Radius, UPARAM(ref) FMapStruct &map);

protected:
	int					GetPos(int x, int y, int quadIndice, FVector2D mapSize);
	inline void			SetVerticeZ(int x, int y, float value, FMapStruct &map);
	void				HLine(int x, int y, int w, float ZValue, FMapStruct &map, TArray<int32> &lookup);
};
