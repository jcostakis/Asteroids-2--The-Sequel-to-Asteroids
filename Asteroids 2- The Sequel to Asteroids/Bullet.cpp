#include "bullet.h"

Bullet::Bullet(sf::Texture& textureAtlas)
{
	speed = 10;

	velocityX = 0;
	velocityY = 0;

	avaliable = true;

	sf::IntRect subRect;
	subRect.left = 64;
	subRect.top = 0;
	subRect.width = 64;
	subRect.height = 64;

	setTexture(textureAtlas);
	setTextureRect(subRect);

	setScale(.5, .5);

	setOrigin(32, 32);

	setPosition(-10000, -10000);
}

/*
	Calculates the x velocity of the bullet using the given angle of the ship.
*/
void Bullet::calculateVelocityX(double angle)
{
	setVelocityX(velocityX +
		std::cos((angle - 90) * M_PI / 180) *
		speed);
}

/*
	Calculates the y velocity of the bullet using the given angle of the ship.
*/
void Bullet::calculateVelocityY(double angle)
{
	setVelocityY(velocityY +
		std::sin((angle - 90) * M_PI / 180) *
		speed);
}