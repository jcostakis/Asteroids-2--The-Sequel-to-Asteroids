#pragma once

#include <stdlib.h>
#include <ctime>
#include <SFML/Graphics.hpp>

class Entity : public sf::Sprite
{
public:
	Entity();
	enum entityType { Ship, Asteroid, Bullet };
	void setSpeed(double newSpeed);
	void setType(entityType newType);
	entityType getType();
	double getSpeed();
	void setVelocity(double newVelocityX, double newVelocityY);
	void setVelocityX(double newVelocityX);
	double getVelocityX();
	void setVelocityY(double newVelocityY);
	double getVelocityY();
	void setMaxSpeed(double newMaxSpeed);
	double getMaxSpeed();
	void setFriction(double newFriction);
	double getFriction();
	void setAvaliable(bool newAvaliable);
	bool getAvaliable();
	void screenCollision();
	void createShip(int windowWidth, int windowHeight, sf::Texture& textureAtlas);
	void shipReset();
	void setInvincible(bool newInvincible);
	bool getInvincible();
	void setDestroyed(bool newDestroyed);
	bool getDestroyed();
	void createBullet(sf::Texture& textureAtlas);
	void resetBullet();
	void createAsteroid(sf::Texture& textureAtlas);
	void resetAsteroid();
	double getRotationSpeed();
	virtual void create(sf::Texture& textureAtlas);
	virtual void reset();
protected:
	entityType type;
	double speed;
	double rotationSpeed;
	double velocityX;
	double velocityY;
	double maxSpeed;
	double friction;
	bool avaliable;
	bool invincible;
	bool destroyed;
	static struct Window window;
};