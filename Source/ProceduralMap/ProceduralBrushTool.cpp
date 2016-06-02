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
UProceduralBrushTool::SetVerticeZ(int x, int y, float value, FMapStruct &map)
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
UProceduralBrushTool::SquareBrush(FVector2D pos, float ZValue, FVector2D BrushSize, UPARAM(ref) FMapStruct &map)
{
	int i, j;

	pos = FVector2D((int)(pos.X / map.MapSquareScale), (int)(pos.Y / map.MapSquareScale));

	for (i = pos.Y; i < pos.Y + BrushSize.Y; i++)
	{
		for (j = pos.X; j < pos.X + BrushSize.X; j++)
		{
			SetVerticeZ(j, i, ZValue, map);
		}
	}
}

void
UProceduralBrushTool::HLine(int x, int y, int w, float ZValue, FMapStruct &map, TArray<int32> &lookup)
{
	int i;
	int pos;

	for (i = 0; i < w; i++)
	{
		pos = (y * map.MapSize.X + (x + i));
		if (!(pos < 0 || pos >= map.Vertices.Num()))
		{
			if (!(lookup.Find(pos) != INDEX_NONE))
			{
				SetVerticeZ(x + i, y, ZValue, map);
				lookup.AddUnique(pos);
			}
		}
	}
}

void
UProceduralBrushTool::CircleBrush(FVector2D pos, float ZValue, int Radius, UPARAM(ref) FMapStruct &map)
{
	TArray<int32> lookup;

	pos = FVector2D((int)(pos.X / map.MapSquareScale), (int)(pos.Y / map.MapSquareScale));

	int xoff = 0;
	int yoff = Radius;
	int balance = -Radius;
	int p0, p1;
	int w0, w1;

	while (xoff <= yoff)
	{
		p0 = pos.X - xoff;
		p1 = pos.X - yoff;

		w0 = xoff + xoff;
		w1 = yoff + yoff;

		HLine(p0, pos.Y + yoff, w0, ZValue, map, lookup);
		HLine(p0, pos.Y - yoff, w0, ZValue, map, lookup);

		HLine(p1, pos.Y + xoff, w1, ZValue, map, lookup);
		HLine(p1, pos.Y - xoff, w1, ZValue, map, lookup);

		if ((balance += xoff++ + xoff) >= 0)
			balance -= --yoff + yoff;
	}
}