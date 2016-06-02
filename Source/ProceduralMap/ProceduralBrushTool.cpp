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
UProceduralBrushTool::SetVerticeZ(int x, int y, FMapStruct &map, int value)
{
	int pos;

	if ((x < 0 || y < 0) || (x >= map.MapSize.X || y >= map.MapSize.Y))
		return ;

	if ((pos = GetPos(x, y, UpLeft, map.MapSize)) != -1)
		map.Vertices[pos].Z += value;
	if ((pos = GetPos(x, y - 1, DownLeft, map.MapSize)) != -1)
		map.Vertices[pos].Z += value;
	if ((pos = GetPos(x - 1, y, UpRight, map.MapSize)) != -1)
		map.Vertices[pos].Z += value;
	if ((pos = GetPos(x - 1, y - 1, DownRight, map.MapSize)) != -1)
		map.Vertices[pos].Z += value;
}

void
UProceduralBrushTool::SquareBrush(FVector2D pos, FMapStruct &map)
{
	int i, j;
	int value = 10;

	for (i = pos.Y; i < pos.Y + BrushSize.Y; i++)
	{
		for (j = pos.X; j < pos.X + BrushSize.X; j++)
		{
			SetVerticeZ(j, i, map, value);
		}
	}
}

void
UProceduralBrushTool::ApplyBrushToMap(FVector2D pos, FMapStruct &map)
{
	FVector2D	relative_pos = FVector2D((int)(pos.X / map.MapSquareScale), (int)(pos.Y / map.MapSquareScale));

	//SquareBrush(relative_pos, map);
	CircleBrush(relative_pos, map, 4);
}

void
UProceduralBrushTool::HLine(int xp, int yp, int w, FMapStruct &map, TArray<int32> &lookup)
{
	int pos;

	for (int i = 0; i < w; i++)
	{
		pos = (yp * map.MapSize.X + (xp + i));
		if (!(pos < 0 || pos >= map.Vertices.Num()))
		{
			if (!(lookup.Find(pos) != INDEX_NONE))
			{
				SetVerticeZ(xp + i, yp, map, 10);
				lookup.AddUnique(pos);
			}
		}
	}
}

void
UProceduralBrushTool::CircleBrush(FVector2D pos, FMapStruct &map, int radius)
{
	TArray<int32> lookup;

	int xoff = 0;
	int yoff = radius;
	int balance = -radius;
	int p0, p1;
	int w0, w1;

	while (xoff <= yoff)
	{
		p0 = pos.X - xoff;
		p1 = pos.X - yoff;

		w0 = xoff + xoff;
		w1 = yoff + yoff;

		HLine(p0, pos.Y + yoff, w0, map, lookup);
		HLine(p0, pos.Y - yoff, w0, map, lookup);

		HLine(p1, pos.Y + xoff, w1, map, lookup);
		HLine(p1, pos.Y - xoff, w1, map, lookup);

		if ((balance += xoff++ + xoff) >= 0)
			balance -= --yoff + yoff;
	}
}