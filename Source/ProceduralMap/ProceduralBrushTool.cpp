// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "ProceduralBrushTool.h"

int	
UProceduralBrushTool::GetPos(int x, int y, int quadIndice, FVector2D mapSize)
{
	if ((x < 0 || y < 0) || (x >= mapSize.X || y >= mapSize.Y))
		return (-1);
	return ((((y * mapSize.X) + x) * 4) + quadIndice);
}

inline void
UProceduralBrushTool::AddVertice(FMapStruct &map, int pos, int value)
{
	if (pos < 0 || pos >= map.Vertices.Num())
		return;
	map.Vertices[pos].Z += value;
}

void
UProceduralBrushTool::SquareBrush(FVector2D pos, FMapStruct &map)
{
	int i, j;
	int value = 10;
	FVector2D brushSize = FVector2D(1, 1);

	//Les 4 coins
	AddVertice(map, GetPos(pos.X - 1, pos.Y - 1, DownRight, map.MapSize), value);
	AddVertice(map, GetPos(pos.X + BrushSize.X, pos.Y - 1, DownLeft, map.MapSize), value);
	AddVertice(map, GetPos(pos.X + BrushSize.X, pos.Y + BrushSize.Y, UpLeft, map.MapSize), value);
	AddVertice(map, GetPos(pos.X - 1, pos.Y + BrushSize.Y, UpRight, map.MapSize), value);
	for (i = pos.Y; i < pos.Y + BrushSize.Y; i++)
	{
		//la "gauche" et la "droite" (x - 1 et x + BrushSizeSizeX)
		AddVertice(map, GetPos(pos.X - 1, i, UpRight, map.MapSize), value);
		AddVertice(map, GetPos(pos.X - 1, i, DownRight, map.MapSize), value);
		AddVertice(map, GetPos(pos.X + BrushSize.X, i, UpLeft, map.MapSize), value);
		AddVertice(map, GetPos(pos.X + BrushSize.X, i, DownLeft, map.MapSize), value);
		for (j = pos.X; j < pos.X + BrushSize.X; j++)
		{
			if (i == pos.Y)
			{
				//le "haut" et le "bas" (y - 1 et y + BrushSizeSizeY)
				AddVertice(map, GetPos(j, pos.Y - 1, DownLeft, map.MapSize), value);
				AddVertice(map, GetPos(j, pos.Y - 1, DownRight, map.MapSize), value);
				AddVertice(map, GetPos(j, pos.Y + BrushSize.Y, UpLeft, map.MapSize), value);
				AddVertice(map, GetPos(j, pos.Y + BrushSize.Y, UpRight, map.MapSize), value);
			}
			//le centre du carré
			AddVertice(map, GetPos(j, i, UpLeft, map.MapSize), value);
			AddVertice(map, GetPos(j, i, UpRight, map.MapSize), value);
			AddVertice(map, GetPos(j, i, DownLeft, map.MapSize), value);
			AddVertice(map, GetPos(j, i, DownRight, map.MapSize), value);
		}
	}
}

void
UProceduralBrushTool::ApplyBrushToMap(FVector2D pos, FMapStruct &map)
{
	FVector2D	relative_pos = FVector2D((int)(pos.X / map.MapSquareScale), (int)(pos.Y / map.MapSquareScale));

	SquareBrush(relative_pos, map);
}