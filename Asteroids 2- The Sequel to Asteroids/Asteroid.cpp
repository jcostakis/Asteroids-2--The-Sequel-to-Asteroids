#include "Asteroid.h"

void Asteroid::create(sf::Texture& textureAtlas)
{
	type = entityType::Asteroid;

	srand(time(0));

	speed = 2;

	rotationSpeed = (((rand() % 60) - 30) / 10);

	velocityX = 0;
	velocityY = 0;

	avaliable = true;

	sf::IntRect subRect;

	//Randomize sprite

	int randomNum = rand() % 2;

	subRect.left = 0;
	subRect.top = 65;
	subRect.width = 128;
	subRect.height = 128;

	setTexture(textureAtlas);
	setTextureRect(subRect);

	//Randomize size
	randomNum = (rand() % 3) + 1;

	setScale(.7 + randomNum / 10, .8 + randomNum / 10);

	setOrigin(64, 64);

	setPosition(-5000, -5000);
}

Asteroid::Asteroid()
{
}

void Asteroid::reset()
{
	setPosition(-5000, -5000);
	setVelocity(0, 0);
	setAvaliable(true);
	rotationSpeed = (((rand() % 60) - 30) / 10);
	speed = ((rand() % 30) + 1) / 10;
}