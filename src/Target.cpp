#include "Target.hpp"

Target::Target(int positionX, int positionY, int textureSize, sf::Texture *texture)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->textureSize = textureSize;
	setSpriteTexture(texture);
	setSpritePosition();
}

Target::~Target() {}
