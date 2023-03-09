#include <cmath>

#include "entity.h"

void Entity::rotateSprite( float angle )
{
	sf::FloatRect rect = this->sprite.getLocalBounds();
	this->sprite.setOrigin( rect.width/2, rect.height/2 );
	this->sprite.rotate( angle );
	this->sprite.setOrigin( 0, 0 );
}

void Entity::setRotation( float angle )
{
	sf::FloatRect rect = this->sprite.getLocalBounds();
	this->sprite.setOrigin( rect.width/2, rect.height/2 );
	sf::Vector2f prevOrigin = sf::Vector2f( -rect.width/2, -rect.height/2);
	float newX, newY;
	float angleR = -angle * acos(-1) / 180;
	newX = prevOrigin.x * cos(angleR) - prevOrigin.y * sin(angleR) + rect.width/2 ;
	newY = prevOrigin.x * sin(angleR) + prevOrigin.y * cos(angleR) + rect.height/2;

	this->sprite.setRotation( angle );
	this->sprite.setOrigin( newX, newY );
}
