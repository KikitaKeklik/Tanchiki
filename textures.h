#pragma once
#include <SFML/Graphics.hpp>

enum class TextureTypes
{
	Bricks, Ground, Water, Unbreakable
};

class TextureGenerator
{

public:
	TextureGenerator( int blocksize = 10);
	~TextureGenerator(){};

	const sf::Texture &GetTexture( TextureTypes );

private:
	float lineThiccness = 0.05;	//in relative unit
	sf::RenderTexture texture;
	int blockSize = 20;
	
};
