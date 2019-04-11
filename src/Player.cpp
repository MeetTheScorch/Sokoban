#include "Player.hpp"

Player::Player(int positionX, int positionY, int textureSize, sf::Texture *texture)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->textureSize = textureSize;
	setSpriteTexture(texture);
	setSpritePosition();
}

Player::~Player() {}

void Player::move(int moveX, int moveY)
{
	this->positionX += moveX;
	this->positionY += moveY;
	setSpritePosition();
}