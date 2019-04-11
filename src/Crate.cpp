#include "Crate.hpp"

Crate::Crate(int positionX, int positionY, int textureSize, sf::Texture *texture)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->textureSize = textureSize;
	setSpriteTexture(texture);
	setSpritePosition();
}

Crate::~Crate() {}

void Crate::move(int moveX, int moveY)
{
	this->positionX += moveX;
	this->positionY += moveY;
	setSpritePosition();
}