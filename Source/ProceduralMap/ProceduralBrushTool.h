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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush")
		FVector2D		BrushSize = FVector2D(1, 1);

public:
	UFUNCTION(BlueprintCallable, Category = "Brush")
		void			SquareBrush(FVector2D pos, UPARAM(ref) FMapStruct &map);

	UFUNCTION(BlueprintCallable, Category = "Brush")
		int				GetPos(int x, int y, int quadIndice, FVector2D mapSize);

	UFUNCTION(BlueprintCallable, Category = "Brush")
		void			ApplyBrushToMap(FVector2D pos, UPARAM(ref) FMapStruct &map);


	void				HLine(int xp, int yp, int w, FMapStruct &map, TArray<int32> &lookup);
	void				CircleBrush(FVector2D pos, UPARAM(ref) FMapStruct &map, int radius);

private:
	inline void AddVertice(FMapStruct &map, int pos, int value);

	inline void AddVertice(FMapStruct &map, int pos, int value, TArray<int32> &lookup);
};
