#include "textures.h"

TextureGenerator::TextureGenerator( int BlockSize )
{
	if (!texture.create( BlockSize, BlockSize  ))
		throw 1;
	this->blockSize = BlockSize;
}

const sf::Texture &TextureGenerator::GetTexture( TextureTypes type)
{
	float nonLineSpace = (1./3 - lineThiccness) * blockSize;
	sf::RectangleShape lineHor( sf::Vector2f( blockSize, lineThiccness * blockSize) );
	sf::RectangleShape lineVert( sf::Vector2f( lineThiccness * blockSize, nonLineSpace ));
	float alpha, x;

	switch( type )
	{
	case (TextureTypes::Bricks):
		texture.clear(sf::Color( 178 , 34, 34 ));
		lineHor.move(0, nonLineSpace);
		for (int i = 0; i < 3; ++i)
		{
			texture.draw(lineHor);
			lineHor.move(0, blockSize/3.);
			alpha =  0.1 + 0.8 * (rand()%10000) / 10000.;
			x = (1 - 2 * lineThiccness)/2 * blockSize;
			lineVert.move(alpha * x , 0);
			texture.draw(lineVert);
			lineVert.move(blockSize/2, 0);
			texture.draw(lineVert);
			lineVert.setPosition(0, (i + 1) * blockSize/3.);
		}
		lineHor.setPosition( 0, 0);
		break;
	case (TextureTypes::Ground):
		texture.clear(sf::Color::Green);
		break;
	case (TextureTypes::Water):
		texture.clear(sf::Color::Blue);
		break;
	default:
		texture.clear(sf::Color::Black);
		break;
	}
	texture.display();
	return texture.getTexture();
}
