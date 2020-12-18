#pragma once

#define _USE_MATH_DEFINES 

#include <cmath>

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet(sf::Texture& textureAtlas);
private:
	void calculateVelocityX(double angle);
	void calculateVelocityY(double angle);
};