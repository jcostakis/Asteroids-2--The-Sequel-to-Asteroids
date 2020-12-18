#pragma once
#include "Entity.h"
class Asteroid : public Entity
{
public:
	void create(sf::Texture& textureAtlas);
	Asteroid();
	void reset();
};

