// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMap.h"
#include "PerlinNoise.h"


PerlinNoise::PerlinNoise(int mapSizeX, int mapSizeY, int res) :
	_MapSizeX(mapSizeX), _MapSizeY(mapSizeY), _Res(res), _MaxValue(255), _Scale(1)
{
	_Map = AllocateMap(mapSizeX, mapSizeY);
}

PerlinNoise::PerlinNoise(const PerlinNoise &src)
	:
	_MapSizeX(src.getMapSizeX()), _MapSizeY(src.getMapSizeY()), _Res(src.getRes()), _MaxValue(src.getMaxValue()), _Scale(src.getScale())
{
	_Map = AllocateMap(_MapSizeX, _MapSizeY);
}

PerlinNoise::~PerlinNoise()
{
	for (int i = 0; i < _MapSizeY; i++)
		delete _Map[i];
	delete _Map;
}

PerlinNoise &
PerlinNoise::operator = (const PerlinNoise &src)
{
	if (this != &src)
	{
		_MapSizeX = src.getMapSizeX();
		_MapSizeY = src.getMapSizeY();
		_Res = src.getRes();
		_Map = src.getMap();
		_MaxValue = src.getMaxValue();
	}
	return (*this);
}

float **
PerlinNoise::AllocateMap(int sizeX, int sizeY)
{
	float	**map = new float*[sizeY];

	for (int i = 0; i < sizeY; i++)
		map[i] = new float[sizeX];
	return (map);
}

float
PerlinNoise::Get2DPerlinNoiseValue(float x, float y, float res)
{
	float tempX, tempY;
	int x0, y0, ii, jj, gi0, gi1, gi2, gi3;
	float unit = 1.0f / sqrt(2);
	float tmp, s, t, u, v, Cx, Cy, Li1, Li2;
	float gradient2[][2] = { { unit,unit },{ -unit,unit },{ unit,-unit },{ -unit,-unit },{ 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

	unsigned int perm[] =
	{ 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
		142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
		203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
		74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
		105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
		187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,
		64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,
		47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,
		153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,
		112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,
		235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
		127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,
		156,180 };

	//Adapter pour la résolution
	x /= res;
	y /= res;

	//On récupère les positions de la grille associée à (x,y)
	x0 = (int)(x);
	y0 = (int)(y);

	//Masquage
	ii = x0 & 255;
	jj = y0 & 255;

	//Pour récupérer les vecteurs
	gi0 = perm[ii + perm[jj]] % 8;
	gi1 = perm[ii + 1 + perm[jj]] % 8;
	gi2 = perm[ii + perm[jj + 1]] % 8;
	gi3 = perm[ii + 1 + perm[jj + 1]] % 8;

	//on récupère les vecteurs et on pondère
	tempX = x - x0;
	tempY = y - y0;
	s = gradient2[gi0][0] * tempX + gradient2[gi0][1] * tempY;

	tempX = x - (x0 + 1);
	tempY = y - y0;
	t = gradient2[gi1][0] * tempX + gradient2[gi1][1] * tempY;

	tempX = x - x0;
	tempY = y - (y0 + 1);
	u = gradient2[gi2][0] * tempX + gradient2[gi2][1] * tempY;

	tempX = x - (x0 + 1);
	tempY = y - (y0 + 1);
	v = gradient2[gi3][0] * tempX + gradient2[gi3][1] * tempY;


	//Lissage
	tmp = x - x0;
	Cx = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

	Li1 = s + Cx*(t - s);
	Li2 = u + Cx*(v - u);

	tmp = y - y0;
	Cy = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

	return Li1 + Cy*(Li2 - Li1);
}

void
PerlinNoise::Generate2DPerlinNoise()
{
	for (int i = 0; i < _MapSizeY; i++)
	{
		for (int j = 0; j < _MapSizeX; j++)
		{
			//_Map[i][j] = (Get2DPerlinNoiseValue(j, i, _Res) + 1) * 0.5 * _MaxValue;
			_Map[i][j] = (Get2DPerlinNoiseValue(j, i, _Res)) * 0.5 * _MaxValue;
			_Map[i][j] *= _Scale;
		}
	}
}

int
PerlinNoise::getMapSizeX(void) const
{
	return (this->_MapSizeX);
}

int
PerlinNoise::getMapSizeY(void) const
{
	return (this->_MapSizeY);
}

int
PerlinNoise::getRes(void) const
{
	return (this->_Res);
}

int
PerlinNoise::getMaxValue(void) const
{
	return (this->_MaxValue);
}

void
PerlinNoise::setMaxValue(int maxValue)
{
	this->_MaxValue = maxValue;
}

float **
PerlinNoise::getMap(void) const
{
	return (_Map);
}

float
PerlinNoise::getScale(void) const
{
	return (this->_Scale);
}

void
PerlinNoise::setScale(float scale)
{
	this->_Scale = scale;
}