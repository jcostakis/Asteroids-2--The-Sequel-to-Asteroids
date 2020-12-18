#include "entity.h"

struct Window
{
	int width = 0;
	int height = 0;
};

struct Window Entity::window;

Entity::Entity()
{
	speed = 0;
}

void Entity::setType(entityType newType)
{
	type = newType;
}

Entity::entityType Entity::getType()
{
	return type;
}

void Entity::setSpeed(double newSpeed)
{
	speed = newSpeed;
}

double Entity::getSpeed()
{
	return speed;
}

void Entity::setVelocity(double newVelocityX, double newVelocityY)
{
	velocityX = newVelocityX;
	velocityY = newVelocityY;
}

void Entity::setVelocityX(double newVelocityX)
{
	velocityX = newVelocityX;
}

double Entity::getVelocityX()
{
	return velocityX;
}

void Entity::setVelocityY(double newVelocityY)
{
	velocityY = newVelocityY;
}

double Entity::getVelocityY()
{
	return velocityY;
}

void Entity::setMaxSpeed(double newMaxSpeed)
{
	newMaxSpeed = maxSpeed;
}

double Entity::getMaxSpeed()
{
	return maxSpeed;
}

void Entity::setFriction(double newFriction)
{
	friction = newFriction;
}

double Entity::getFriction()
{
	return friction;
}

void Entity::setAvaliable(bool newAvaliable)
{
	avaliable = newAvaliable;
}

bool Entity::getAvaliable()
{
	return avaliable;
}

/*
	Checks if this entity is offscreen and corrects.
*/
void Entity::screenCollision()
{
	//Check top
	if (getPosition().y < -31)
	{
		//If asteroid
		if (getType() == entityType::Asteroid)
		{
			resetAsteroid();
		}

		//If bullet
		else if (getType() == entityType::Bullet)
		{
			resetBullet();
		}

		//If ship
		else if (getType() == entityType::Ship)
		{
			setPosition(getPosition().x, window.height + 30);
		}
	}

	//Check bottom
	if (getPosition().y > window.height + 31)
	{
		//If asteroid . TODO: WHY IS THIS DIFFERENT
		if (getType() == entityType::Asteroid && getPosition().y > window.height + 301)
		{
			resetAsteroid();
		}

		//If bullet
		else if (getType() == entityType::Bullet)
		{
			resetBullet();
		}

		//If ship
		else if (getType() == entityType::Ship)
		{
			setPosition(getPosition().x, -30);
		}
	}

	//Check left
	if (getPosition().x < -30)
	{
		//If asteroid
		if (getType() == entityType::Asteroid && getPosition().x < -301)
		{
			resetAsteroid();
		}

		//If bullet
		else if (getType() == entityType::Bullet)
		{
			resetBullet();
		}

		//If ship
		else if (getType() == entityType::Ship)
		{
			setPosition(window.width + 29, getPosition().y);
		}
	}

	//Check right
	if (getPosition().x > window.width + 30)
	{
		//If asteroid
		if (getType() == entityType::Asteroid && getPosition().x > window.width + 301)
		{
			resetAsteroid();
		}

		//If bullet
		else if (getType() == entityType::Bullet)
		{
			resetBullet();
		}

		//If ship
		else if (getType() == entityType::Ship)
		{
			setPosition(-31, getPosition().y);
		}
	}
}

void Entity::createShip(int windowWidth, int windowHeight, sf::Texture& textureAtlas)
{
	// Sets the variable for every instance
	Entity::window.width = windowWidth;
	Entity::window.height = windowHeight;

	type = entityType::Ship;

	speed = 0.05;
	maxSpeed = 15;

	velocityX = 0;
	velocityY = 0;

	friction = 0.025;

	sf::IntRect subRect;
	subRect.left = 0;
	subRect.top = 0;
	subRect.width = 64;
	subRect.height = 64;

	setTexture(textureAtlas);
	setTextureRect(subRect);

	setScale(.5, .5);

	setOrigin(sf::Vector2f(32, 32));

	shipReset();
}

void Entity::shipReset()
{
	setPosition(-2500, -2500);
	setDestroyed(true);
	setInvincible(true);
}

void Entity::setInvincible(bool newInvincible)
{
	invincible = newInvincible;

	if (newInvincible == false)
	{
		//Makes it opaque again
		setColor(sf::Color(255, 255, 255, 255));
	}
}

bool Entity::getInvincible()
{
	return invincible;
}

void Entity::setDestroyed(bool newDestroyed)
{
	if (newDestroyed == true)
	{
		setColor(sf::Color(255, 255, 255, 0));
	}
	else
	{
		setColor(sf::Color(255, 255, 255, 75));
		setVelocity(0, 0);
		setRotation(0);
		setPosition(window.width / 2, window.height / 2);
	}

	destroyed = newDestroyed;
}

bool Entity::getDestroyed()
{
	return destroyed;
}

void Entity::createBullet(sf::Texture& textureAtlas)
{
	type = entityType::Bullet;

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

void Entity::resetBullet()
{
	setPosition(-10000, -10000);
	setVelocity(0, 0);
	setAvaliable(true);
}

void Entity::createAsteroid(sf::Texture& textureAtlas)
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

void Entity::resetAsteroid()
{
	setPosition(-5000, -5000);
	setVelocity(0, 0);
	setAvaliable(true);
	rotationSpeed = (((rand() % 60) - 30) / 10);
	speed = ((rand() % 30) + 1) / 10;
}

double Entity::getRotationSpeed()
{
	return rotationSpeed;
}

void Entity::reset() {
}

void Entity::create(sf::Texture& textureAtlas) {
}