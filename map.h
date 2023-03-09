#pragma once

class Map
{
public:
	Map();
	Map(int , int);
	Map( const char* );
	~Map();

	char getBlock( int x, int y );

	void setBlock( int, int, char );

	int getWidth( void ) { return this->width; };
	int getHeight( void ) { return this->height; };
protected:
	char** map = nullptr;
	int height = 0, width = 0;
};
