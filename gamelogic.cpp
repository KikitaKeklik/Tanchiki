#include <cmath>
#include <iostream>

#include "gamelogic.h"

void GameLogic::addPlayer( sf::Sprite spr, sf::Vector2f pos, sf::Vector2f dir, int life )
{
	playerList.push_back( Player(spr, pos, life) );
	playerList.back().getSprite().setPosition( pos.x * this->BlockSize, pos.y * this->BlockSize );
	playerList.back().setDir(dir);
	
	if (playerList.size() != 1 )
		playerList.back().setId(playerList.size() - 1); // TODO fix ids
}

void GameLogic::addShot( sf::Sprite spr, sf::Vector2f pos, int id)
{
	shotList.push_back( Shot(spr, pos) );
	shotList.back().setShooterId( id );
}

void GameLogic::movePlayer( int i, sf::Vector2f dir )
{
	if (playerList[i].isMoving())
		return;
	playerList[i].setDir(dir);
	float angle = atan2f(dir.x, -dir.y) * 180. / acos(-1) ;
	playerList[i].setRotation( angle );

	sf::Vector2f pos = playerList[i].getPos();
	pos += dir;
	if ( !(this->getBlock((int)pos.x, (int)pos.y) & BlockTypes::CanMove ) )
		return;
	for (int j = 0; j < playerList.size(); ++j)
		if ( (playerList[j].getPos() == playerList[i].getPos() + dir) && (i != j) )
			return;
	playerList[i].setMoving(true);
	playerList[i].timeFromMov = 0;
}

void GameLogic::shootFromPlayer( int i )
{
	if (!playerList[i].getShootState())
		return;
	playerList[i].setShootState(false);
	playerList[i].timeFromShot = 0;
	shotList.push_back(Shot( shotSprite, sf::Vector2f(playerList[i].getPos()) ) );
	shotList.back().getSprite().setPosition(playerList[i].getPos().x * this->BlockSize, playerList[i].getPos().y * this->BlockSize );
	shotList.back().setShooterId( playerList[i].getId() );
	shotList.back().setDir( playerList[i].getDir() );
}

void GameLogic::update( double frameTime )
{
	for (int i = 0; i < shotList.size(); ++i )
	{
		shotList[i].timeFromMov += frameTime;
		if (shotList[i].timeFromMov >= shotList[i].getSpeed())
		{
			shotList[i].getPos() += shotList[i].getDir();
			shotList[i].setOffset( sf::Vector2f(0, 0) );
			shotList[i].timeFromMov = 0;
		}
		else
		{
			float delta = frameTime * this->BlockSize / shotList[i].getSpeed();
			shotList[i].getOffset() += delta * shotList[i].getDir();
		}
		shotList[i].getSprite().setPosition( (float)this->BlockSize * shotList[i].getPos() + shotList[i].getOffset());
	}

	auto it = shotList.begin();
	sf::Vector2f pos;
	for (int i = 0; i < shotList.size(); ++i, ++it)
	{
		pos = shotList[i].getPos();
		char currBlock = this->getBlock( (int) pos.x, (int) pos.y );
		if ( !(currBlock & BlockTypes::CanShotOver) )
		{
			if ( currBlock & BlockTypes::CanBreak )
			{
				char blockHP = currBlock>>3;
				blockHP--;
				currBlock &= ~hpMask;
				currBlock += blockHP<<3;
				currBlock = blockHP == 0 ? BlockTypes::CanMove | BlockTypes::CanShotOver : currBlock;
				this->setBlock( (int)pos.x, (int) pos.y, currBlock );
			}
			shotList.erase( it );
			continue;
		}
		pos += shotList[i].getDir();
		currBlock = this->getBlock( (int) pos.x, (int) pos.y );
		if ( !(currBlock & BlockTypes::CanShotOver) )
		{
			if ( currBlock & BlockTypes::CanBreak )
			{
				char blockHP = currBlock >> 3;
				blockHP--;
				currBlock &= ~hpMask;
				currBlock += blockHP<<3;
				currBlock = blockHP == 0 ? BlockTypes::CanMove | BlockTypes::CanShotOver : currBlock;
				this->setBlock( (int)pos.x, (int) pos.y, currBlock );
			}
			shotList.erase( it );
			continue;
		}
	}

	auto it1 = playerList.begin();
	auto it2 = shotList.begin();
	int hp;
	bool flag = false;
	for (int i = 0; i < shotList.size(); ++i, ++it2)
	{
		flag = false;
		for (int j = 0; j < playerList.size(); ++j, ++it1)
		{
			if ( (playerList[j].getPos() == shotList[i].getPos()) && ( playerList[j].getId() != shotList[i].getShooterId() ) )
			{
				flag = true;
				hp = playerList[j].getLife();
				hp--;
				if (!hp)
				{
					playerList.erase(it1);
					break;
				}
				playerList[j].setLife(hp);
				break;	
			}
		}
		if (flag)
			shotList.erase(it2);
	}

	for (int i = 0; i < playerList.size(); ++i )
	{
		playerList[i].timeFromShot += frameTime;
		if (playerList[i].isMoving())
		{
			playerList[i].timeFromMov += frameTime;
			if (playerList[i].timeFromMov >= playerList[i].getSpeed())
			{
				playerList[i].getPos() += playerList[i].getDir();
				playerList[i].setOffset( sf::Vector2f(0, 0) );
				playerList[i].setMoving( false );
			}
			else
			{
				float delta = frameTime * this->BlockSize / playerList[i].getSpeed();
				playerList[i].getOffset() += delta * playerList[i].getDir();
			}
			playerList[i].getSprite().setPosition( (float)this->BlockSize * playerList[i].getPos() + playerList[i].getOffset());
		}
	}
}

void GameLogic::setBlockSize( int newSize )
{
	for ( int i = 0; i < this->playerList.size(); ++i)
	{
		playerList[i].setOffset( (float)newSize / this->BlockSize * this->playerList[i].getOffset() );
		playerList[i].getSprite().setPosition( playerList[i].getPos() * (float)newSize);
	}

	for ( int i = 0; i < this->shotList.size(); ++i){
		shotList[i].setOffset( (float)newSize / this->BlockSize * this->shotList[i].getOffset() );
		shotList[i].getSprite().setPosition( shotList[i].getPos() * (float)newSize);
	}
	this->BlockSize = newSize;
}
