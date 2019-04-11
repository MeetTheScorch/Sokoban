#pragma once

class Player
{
  public:
	Player(int, int, int, sf::Texture *);
	~Player();

	int getPositionX() { return this->positionX; }
	void setPositionX(int positionX) { this->positionX = positionX; }
	int getPositionY() { return this->positionY; }
	void setPositionY(int positionY) { this->positionY = positionY; }

	void move(int, int);

	sf::Sprite &getSprite() { return this->sprite; }
	void setSpriteTexture(sf::Texture *texture) { this->sprite.setTexture(*texture); }
	void setSpritePosition() { sprite.setPosition(getPositionX() * textureSize, getPositionY() * textureSize); }

  private:
	int positionX, positionY;
	int textureSize;
	sf::Sprite sprite;
};