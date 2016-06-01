// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "ProceduralBrushTool.h"

int	
UProceduralBrushTool::GetPos(int x, int y, int quadIndice, int sizeX)
{
	if (x < 0 || y < 0)
		return (-1);
	return ((((y * sizeX) + x) * 4) + quadIndice);
}

void
UProceduralBrushTool::SetVertice(TArray<int32> &lst, int pos)
{
	if (pos < 0)
		return;
	lst.Add(pos);
}

TArray<int32>
UProceduralBrushTool::SquareBrush(FVector2D pos, FVector2D brushSize, FVector2D mapSize)
{
	int i, j;

	TArray<int32>	verticesToEdit;

	//Les 4 coins
	SetVertice(verticesToEdit, GetPos(pos.X - 1, pos.Y - 1, DownRight, mapSize.X));
	SetVertice(verticesToEdit, GetPos(pos.X + brushSize.X, pos.Y - 1, DownLeft, mapSize.X));
	SetVertice(verticesToEdit, GetPos(pos.X + brushSize.X, pos.Y + brushSize.Y, UpLeft, mapSize.X));
	SetVertice(verticesToEdit, GetPos(pos.X - 1, pos.Y + brushSize.Y, UpRight, mapSize.X));

	for (i = pos.Y; i < pos.Y + brushSize.Y; i++)
	{
		//la "gauche" et la "droite" (x - 1 et x + BrushSizeSizeX)
		SetVertice(verticesToEdit, GetPos(pos.X - 1, i, UpRight, mapSize.X));
		SetVertice(verticesToEdit, GetPos(pos.X - 1, i, DownRight, mapSize.X));
		SetVertice(verticesToEdit, GetPos(pos.X + brushSize.X, i, UpLeft, mapSize.X));
		SetVertice(verticesToEdit, GetPos(pos.X + brushSize.X, i, DownLeft, mapSize.X));
		for (j = pos.X; j < pos.X + brushSize.X; j++)
		{
			if (i == pos.Y)
			{
				//le "haut" et le "bas" (y - 1 et y + BrushSizeSizeY)
				SetVertice(verticesToEdit, GetPos(j, pos.Y - 1, DownLeft, mapSize.X));
				SetVertice(verticesToEdit, GetPos(j, pos.Y - 1, DownRight, mapSize.X));
				SetVertice(verticesToEdit, GetPos(j, pos.Y + brushSize.Y, UpLeft, mapSize.X));
				SetVertice(verticesToEdit, GetPos(j, pos.Y + brushSize.Y, UpRight, mapSize.X));
			}
			//le centre du carré
			SetVertice(verticesToEdit, GetPos(j, i, UpLeft, mapSize.X));
			SetVertice(verticesToEdit, GetPos(j, i, UpRight, mapSize.X));
			SetVertice(verticesToEdit, GetPos(j, i, DownLeft, mapSize.X));
			SetVertice(verticesToEdit, GetPos(j, i, DownRight, mapSize.X));
		}
	}
	return (verticesToEdit);
}
