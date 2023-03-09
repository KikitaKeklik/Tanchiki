#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "textures.h"
#include "gamelogic.h"

int BlockSize = 20;
float MaxFPS = 100;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Tanks v0.1", sf::Style::Titlebar | sf::Style::Close);

	TextureGenerator genGrd(BlockSize), genWat(BlockSize), genBr(BlockSize), genUnb(BlockSize);
	sf::Sprite groundSprite( genGrd.GetTexture( TextureTypes::Ground ) );
	sf::Sprite waterSprite( genWat.GetTexture( TextureTypes::Water ) );
	sf::Sprite brickSprite( genBr.GetTexture( TextureTypes::Bricks ) );
	sf::Sprite undestroyableSprite( genUnb.GetTexture( TextureTypes::Unbreakable ) );

	sf::RenderTexture tankTexture;
	tankTexture.create(BlockSize, BlockSize);
	tankTexture.clear(sf::Color::Transparent);

	sf::RectangleShape elem1( sf::Vector2f(0.15 * BlockSize, 0.7 * BlockSize) );
	elem1.setOutlineThickness( -1.f);
	elem1.setOutlineColor( sf::Color::Black );
	sf::RectangleShape body( sf::Vector2f(0.7 * BlockSize, 0.8 * BlockSize) );
	body.setOutlineThickness( -1.f );
	body.setOutlineColor( sf::Color::Black );
	sf::RectangleShape head( sf::Vector2f( 0.3 * BlockSize, 0.3 * BlockSize ) );
	head.setOutlineThickness( -1.f );
	head.setOutlineColor( sf::Color::Black );
	sf::RectangleShape gun( sf::Vector2f( 0.1 * BlockSize, 0.55 * BlockSize) );
	gun.setOutlineThickness( -1.f );
	gun.setOutlineColor( sf::Color::Black );
	head.setPosition( 0.35 * BlockSize, 0.45 * BlockSize);
	gun.setPosition( 0.45 * BlockSize, 0 );

	elem1.setPosition( 0, 0.25 * BlockSize );
	tankTexture.draw( elem1 );
	elem1.setPosition( 0.85 * BlockSize, 0.25 * BlockSize );
	tankTexture.draw( elem1 );
	body.setPosition( 0.15 * BlockSize, 0.2 * BlockSize );
	tankTexture.draw( body );
	tankTexture.draw(head);
	tankTexture.draw(gun);
	tankTexture.display();
	sf::Sprite tankSprite( tankTexture.getTexture());
	tankSprite.setColor( sf::Color( 10, 190, 10 ) );

	sf::RenderTexture projTexture;
	projTexture.create(BlockSize, BlockSize);
	projTexture.clear(sf::Color::Transparent);

	sf::RectangleShape projBody( sf::Vector2f( 0.3 * BlockSize, (1-0.3 * sqrt(3))/2 * BlockSize ) );
	projBody.setOutlineThickness( -1.f);
	projBody.setOutlineColor( sf::Color::Black );
	sf::CircleShape projHead( 0.3 * BlockSize / sqrt(3), 3 );
	projHead.setOutlineThickness( -1.f);
	projBody.setOutlineColor( sf::Color::Black);

	projBody.setPosition( 0.35*BlockSize, (1./4 + 0.3*sqrt(3)/2) * BlockSize);
	projTexture.draw( projBody );

	projHead.setPosition( (0.5 - 0.3/sqrt(3)) * BlockSize, BlockSize/4. );
	projTexture.draw( projHead );

	projTexture.display();

	GameLogic map( "maps/1.map" );
	map.addPlayer( tankSprite, sf::Vector2f(1, 1), sf::Vector2f(0, -1), 3 );
	map.addPlayer( tankSprite, sf::Vector2f(3, 3), sf::Vector2f(0, -1), 3 );
	map.setBlockSize(BlockSize);
	map.shotSprite = sf::Sprite( projTexture.getTexture() );
	map.shotSprite.setColor( sf::Color( 100, 10, 10 ) );

	sf::Clock timer;
	float frameTime;
	bool moveLeft = false, moveRight = false, moveUp = false, moveDown = false, shoot = false;

	while (window.isOpen())
	{
		frameTime = timer.restart().asMilliseconds()/1000.;
		map.update( frameTime < 1./MaxFPS ? 1./MaxFPS : frameTime);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type){
			case (sf::Event::Closed):
				window.close();
				break;

			case (sf::Event::KeyPressed):
				switch( event.key.code )
				{
					case sf::Keyboard::Escape:
						window.close();
						break;

					case sf::Keyboard::A:
					case sf::Keyboard::Left:
						moveLeft = true;
						break;
					case sf::Keyboard::W:
					case sf::Keyboard::Up:
						moveUp = true;
						break;
					case sf::Keyboard::D:
					case sf::Keyboard::Right:
						moveRight = true;
						break;
					case sf::Keyboard::S:
					case sf::Keyboard::Down:
						moveDown = true;
						break;
					case sf::Keyboard::Space:
						shoot = true;
						break;
				}
				break;
			case (sf::Event::KeyReleased):
				switch( event.key.code )
				{
					case sf::Keyboard::A:
					case sf::Keyboard::Left:
						moveLeft = false;
						break;
					case sf::Keyboard::W:
					case sf::Keyboard::Up:
						moveUp = false;
						break;
					case sf::Keyboard::D:
					case sf::Keyboard::Right:
						moveRight = false;
						break;
					case sf::Keyboard::S:
					case sf::Keyboard::Down:
						moveDown = false;
						break;
					case sf::Keyboard::Space:
						shoot = false;
						break;
				}
				break;
			}
		}

		if (moveLeft)
			map.movePlayer( 0, sf::Vector2f( -1, 0 ) );
		else if (moveUp)
			map.movePlayer( 0, sf::Vector2f( 0, -1 ) );
		else if (moveRight)
			map.movePlayer( 0, sf::Vector2f( 1, 0 ) );
		else if (moveDown)
			map.movePlayer( 0, sf::Vector2f( 0, 1 ) );

		if (shoot)
			map.shootFromPlayer( 0 );

		window.clear(sf::Color::Black);
		char currBlock;	
		for (int i = 0; i < map.getHeight(); ++i)
		{
			for (int j = 0; j < map.getWidth(); ++j)
			{
				currBlock = map.getBlock(i, j);
				if (currBlock & BlockTypes::CanMove)
				{
					groundSprite.setPosition( i * BlockSize, j * BlockSize );
					window.draw( groundSprite );
				}
				else if ( currBlock & BlockTypes::CanShotOver)
				{	
					waterSprite.setPosition( i * BlockSize, j * BlockSize );
					window.draw( waterSprite );
				}
				else if( currBlock & BlockTypes::CanBreak)
				{	
					brickSprite.setPosition( i * BlockSize, j * BlockSize );
					window.draw( brickSprite);
				}
				else
				{
					undestroyableSprite.setPosition( i * BlockSize, j * BlockSize );
					window.draw( undestroyableSprite );
				}
			}
		}

		for ( int i = 0; i < map.getPlayerNumber(); ++i){
			window.draw(map.getPlayer(i)->getSprite());
		}
		for (int i = 0; i < map.getShotNumber(); ++i)
		{
			window.draw( map.getShot(i)->getSprite());
		}
		window.display();

		if (frameTime < 1./MaxFPS)
			sf::sleep( sf::milliseconds( 1000. * (1./MaxFPS - frameTime ) ));

	}

	return 0;
}
