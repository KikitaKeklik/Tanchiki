#include <stdio.h>

#include "map.h"

Map::Map()
{
}

Map::Map( int width, int height )
{
	map = new char*[height];
	for (int i = 0; i < height; ++i)
		map[i] = new char[width];
	this->height = height;
	this->width = width;
}

Map::Map( const char* fileName)
{
	FILE* file = fopen(fileName, "r");

	fscanf(file, "%d %d\n", &this->width, &this->height);

	map = new char*[this->height];
	for (int i = 0; i < this->height; ++i )
		map[i] = new char[this->width];

	for (int i = 0; i < this->height; ++i )
	{
		fscanf(file, "\n");
		for (int j = 0; j < this->width; ++j )
				fscanf(file, "%hhd ", &map[i][j]);
	}

	fclose(file);
}

Map::~Map()
{
	if (map == nullptr)
		return;
	
	for (int i = 0; i < height; ++i)
		delete map[i];
	delete map;
}

char Map::getBlock( int x, int y )
{
	if ( (x<0) || (x >= width) || (y<0) || (y >= height) )
		return 0;
	return map[y][x];
}

void Map::setBlock( int x, int y, char type )
{
	if ( ( x < 0) || (x >= width) || ( y < 0 ) || ( y >= height ) )
		return;

	map[y][x] = type;
}
