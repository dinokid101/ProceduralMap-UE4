// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <cmath>
/**
 * 
 */
class PROCEDURALMAP_API PerlinNoise
{
protected:
	int	_MapSizeX;
	int	_MapSizeY;
	float _Res;
	int _MaxValue;
	float **_Map;
	float _Scale;

public:
	PerlinNoise(int mapSizeX, int mapSizeY, int res);
	PerlinNoise(const PerlinNoise &src);
	~PerlinNoise();

public:
	PerlinNoise &operator = (const PerlinNoise &src);

protected:
	float		**AllocateMap(int sizeX, int sizeY);

public:
	float		Get2DPerlinNoiseValue(float x, float y, float res);
	void		Generate2DPerlinNoise();

public:
	int			getMapSizeX(void) const;
	int			getMapSizeY(void) const;
	int			getRes(void) const;
	int			getMaxValue(void) const;
	void		setMaxValue(int maxValue);
	float		**getMap(void) const;
	float		getScale(void) const;
	void		setScale(float scale);
};
