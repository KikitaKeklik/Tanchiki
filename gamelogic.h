#pragma once
#include <vector>

#include "map.h"
#include "entity.h"

enum BlockTypes
{
	CanMove = 1,			//can you move on this block
	CanShotOver = 2,	//can your shot fly over this block( water)
	CanBreak = 4			//cam you break this block
};

class GameLogic : public Map
{
public:
	GameLogic( int width, int height):Map(width,height){};
	GameLogic( const char* fileName):Map(fileName){};

	void setBlockSize( int newSize );
	int getBlockSize( void ) { return this->BlockSize; };

	void addPlayer( sf::Sprite, sf::Vector2f pos, sf::Vector2f dir = sf::Vector2f(0, -1), int life = 3);
	void addShot( sf::Sprite, sf::Vector2f pos, int);

	int getPlayerNumber( void ) { return this->playerList.size(); };
	int getShotNumber( void ) { return this->shotList.size(); };

	Player *getPlayer( int i ) { if ( ( i<0 ) || (i >= this->playerList.size())) return nullptr; return &playerList[i]; };
	Shot *getShot( int i ) { if (( i < 0 ) || ( i >= this->shotList.size())) return nullptr; return &shotList[i]; };

	void movePlayer(int i, sf::Vector2f dir);
	void shootFromPlayer( int i );

	void update( double pastTime );

	sf::Sprite shotSprite;

protected:
	int BlockSize = 20;
	char hpMask = 0b11111000;
	std::vector<Player> playerList;
	std::vector<Shot> shotList;
};
