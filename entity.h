#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum Direction
{
	Up,
	Right,
	Down,
	Left
};

class Entity
{
public:
	Entity() {};

	Entity( sf::Texture texture) { this->sprite = sf::Sprite(texture); };
	Entity( sf::Texture texture, int life):Entity(texture) { this->life = life;};
	Entity( sf::Texture texture, sf::Vector2f pos):Entity(texture) { this->pos = pos; };
	Entity( sf::Texture texture, sf::Vector2f pos, int life):Entity( texture, pos) {this->life = life;};

	Entity( sf::Sprite spr) { this->sprite = spr; };
	Entity( sf::Sprite spr, int life): Entity(spr) { this->life = life;};
	Entity( sf::Sprite spr, sf::Vector2f pos) : Entity( spr ) { this->pos = pos; };
	Entity( sf::Sprite spr, sf::Vector2f pos, int life): Entity( spr, pos) { this->life = life;};

	void rotateSprite( float angle );
	void setRotation( float angle );

	sf::Sprite &getSprite( void ) { return this->sprite; };
	int getLife( void ) { return this->life;};
	sf::Vector2f &getOffset( void ) { return this->offset; };
	sf::Vector2f &getPos( void ) { return this->pos; };

	void setSprite( sf::Sprite newSprite ) { this->sprite = newSprite;};
	void setLife( int newLife) { this->life = newLife; };
	void setOffset( sf::Vector2f newOff) { this->offset = newOff; };
	void setPos( sf::Vector2f newPos ) { this->pos = newPos; };

	void move( sf::Vector2f chngOff) { this->offset += chngOff; };
	bool isMoving( void ) { return this->inMotion; };
	void setMoving( bool state ) { this->inMotion = state; };

	void setDir( sf::Vector2f dir ) { this->dir = dir; setRotation( atan2( dir.x, -dir.y) * 180 / acos(-1)); };
	sf::Vector2f &getDir( void ) { return this->dir; };
	
protected:
	sf::Sprite sprite;
	sf::Vector2f pos = sf::Vector2f(0, 0);
	sf::Vector2f offset = sf::Vector2f(0, 0);
	int life = 1;
	char inMotion = false;
	sf::Vector2f dir = sf::Vector2f(0, -1);
};

class Player: public Entity
{

public:
	Player( sf::Sprite spr, sf::Vector2f pos, int life = 3):Entity(spr, pos, life) {  };
	void setName( const char* name) { this->name = name; };
	void setId( int id ) { this->id = id; };

	void setSpeed( float sp) { this->speed = sp; };
	float getSpeed( void ) { return this->speed; };

	void setShootState( bool state ) { this->canShoot = state;};
	bool getShootState( void ) { this->canShoot = timeFromShot > CD; return this->canShoot; };
	
	void setCD( float CD ) { this->CD = CD; };
	float getCD( void ) { return this->CD; };

	int getId( void ) { return this->id; };
	const char* getName( void ) { return this->name; };
	float timeFromMov = 0, timeFromShot = 0;

protected:
	int id = 0;
	const char* name;
	float speed = 1; // time for passing 1 block
	float CD = 1.5;
	bool canShoot = true;
};

class Shot: public Entity
{
public:
	Shot( sf::Sprite spr, sf::Vector2f pos ): Entity(spr, pos ) {};

	int getShooterId( void ){ return this->shooterId; };
	void setShooterId( int newId ){ this->shooterId = newId;};

	float getSpeed( void ) { return this->speed; };
	void setSpeed( float sp) { this->speed = sp; };

	float timeFromMov = 0;

protected:
	int shooterId = 0;
	float speed = 0.2;
};
