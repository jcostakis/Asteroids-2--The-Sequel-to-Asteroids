//Starts the program

#define _USE_MATH_DEFINES 

#include <stdlib.h>
#include <iostream>

#include <string>
#include <sstream>
#include <vector>

#include <ctime>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "entity.h"

const int shipIndex = 0;

const int bulletStartIndex = 1;
const int bulletStopIndex = 21;

const int asteroidStartIndex = 21;
const int asteroidStopIndex = 61;

//Returns avaliable bullet.
int requestBullet(std::vector<Entity> entities)
{
	for (int i = bulletStartIndex; i < bulletStopIndex; i++)
	{
		if (entities[i].getAvaliable())
		{
			return i;
		}
	}

	std::cerr << "you're shooting too fast! stop it!!!!!\n\n";
	system("pause");
	exit(1);
}

//Returns avaliable asteroid.
int requestAsteroid(std::vector<Entity> entities)
{
	for (int i = asteroidStartIndex; i < asteroidStopIndex; i++)
	{
		if (entities[i].getAvaliable())
		{
			return i;
		}
	}

	std::cerr << "too many asteroids. you broke it!";
	system("pause");
	exit(2);
}

int main()
{
	//Creating window
	int antialiasing = 16;

	sf::ContextSettings settings;
	settings.antialiasingLevel = antialiasing;

	int windowWidth = 1024, windowHeight = 768;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Asteroids",
		sf::Style::Titlebar | sf::Style::Close, settings);

	window.setFramerateLimit(100);
	window.setKeyRepeatEnabled(false);

	// create a view with its center and size
	sf::View view(sf::Vector2f(windowWidth / 2, windowHeight / 2), sf::Vector2f(windowWidth, windowHeight));

	window.setView(view);

	//Stream for ui
	std::ostringstream stream;

	//Lives text
	sf::Font font;
	if (!font.loadFromFile("GRUPO3.otf"))
	{
		std::cerr << "The font file is missing./n";
		system("pause");
		exit(3);
	}

	//Text for lives in UI
	sf::Text lives;

	lives.setFont(font);
	lives.setCharacterSize(24);
	lives.setPosition(50, 20);
	lives.setFillColor(sf::Color(200, 50, 75, 255));

	//Text for score in UI
	sf::Text scoreText;

	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setPosition(windowWidth - 125, 20);
	scoreText.setFillColor(sf::Color(200, 50, 75, 255));

	//Seed random
	srand(time(0));

	//How often the physics should be updated
	sf::Time physicsRate = sf::milliseconds(10);

	//Tracks accumulated time 
	sf::Time accumulator = sf::milliseconds(0);

	//How often asteroids should be produced
	sf::Time asteroidRate = sf::milliseconds(3000);//3 sec

	//Tracks time for asteroids
	sf::Time asteroidTimer = sf::milliseconds(0);

	//How long invincibility should last
	sf::Time invincibilityLength = sf::milliseconds(4000);//4 sec

	//Tracks time for invincibility
	sf::Time invincibilityTimer = sf::milliseconds(0);

	//How long ship should stay destroyed
	sf::Time destroyedLength = sf::milliseconds(500);//0.5 sec

	//Tracks time for destruction
	sf::Time destroyedTimer = sf::milliseconds(0);

	//Tracks time since game start
	sf::Time gameTimer = sf::milliseconds(0);

	//Tracks time between frames
	sf::Clock clock;

	//Vector holding all entities
	std::vector<Entity> entities(100);

	//Loading texture atlas
	sf::Texture textureAtlas;
	if (!textureAtlas.loadFromFile("textureAtlas.png"))
	{
		//error
	}

	textureAtlas.setSmooth(true);

	//Creation of ship
	entities[shipIndex].createShip(windowWidth, windowHeight, textureAtlas);

	//Creation of bullets
	for (int i = bulletStartIndex; i < bulletStopIndex; i++)
	{
		entities[i].createBullet(textureAtlas);
	}

	//Creation of asteroids
	for (int i = asteroidStartIndex; i < asteroidStopIndex; i++)
	{
		entities[i].createAsteroid(textureAtlas);
	}

	bool upPressed = false, rightPressed = false,
		leftPressed = false, spacePressed = false;

	int playerLives = 3, playerScore = 0, flicker = 0;

	//Game loop
	while (window.isOpen())
	{
		//Input

		//Checks Events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{

				//window close request: window closes
			case sf::Event::Closed:
				window.close();
				break;

				//key pressed: check which key was pressed
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{

					//up key pressed: turn upPressed bool on for logic
				case sf::Keyboard::Up:
					upPressed = true;
					break;

					//right key pressed: turn rightPressed bool on for logic
				case sf::Keyboard::Right:
					rightPressed = true;
					break;

					//left key pressed: turn leftPressed bool on for logic
				case sf::Keyboard::Left:
					leftPressed = true;
					break;

					//space key pressed: turn spacePressed bool on for logic
				case sf::Keyboard::Space:
					spacePressed = true;
					break;
				}
				break;

				//key released: check which key was released
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{

					//up key released: turn rightPressed bool off for logic
				case sf::Keyboard::Up:
					upPressed = false;
					break;

					//right key released: turn rightPressed bool off for logic
				case sf::Keyboard::Right:
					rightPressed = false;
					break;

					//right key released: turn rightPressed bool off for logic
				case sf::Keyboard::Left:
					leftPressed = false;
					break;

					//space key released: turn spacePressed bool off for logic
				case sf::Keyboard::Space:
					spacePressed = false;
					break;
				}
				break;
			}
		}

		//end Input

		//Logic

		//Physics- physics rate seperated from framerate

		//Gets time since last frame
		accumulator += clock.restart();

		//Use accumulator times to track whether asteroid should be reproduced

		asteroidTimer += accumulator;

		//Use accumulator times to track invincibilty
		if (entities[0].getInvincible() && !entities[0].getDestroyed())
		{
			invincibilityTimer += accumulator;
		}

		//Use accumulator times to track destruction
		if (entities[0].getDestroyed())
		{
			destroyedTimer += accumulator;
		}

		//Use accumulator times to track how long the player has been playing

		gameTimer += accumulator;

		//While accumulated time is greater than the physics rate
		while (accumulator >= physicsRate)
		{
			//Collision

			//Check if entities are off screen
			for (int i = 0; i < entities.size(); i++)
			{
				entities[i].screenCollision();

				//Check if colliding with other entities

				//Check if ship collides with asteroid
				if (i == 0 && !entities[i].getInvincible())
				{
					for (int j = 21; j < 61; j++)
					{
						//Checks if center is below top of asteroid
						if (entities[i].getPosition().y > entities[j].getPosition().y - (entities[j].getGlobalBounds().height / 3.5))
						{
							//Checks if center is above bottom of asteroid
							if (entities[i].getPosition().y < entities[j].getPosition().y + (entities[j].getGlobalBounds().height / 3.5))
							{
								//Checks if center is to the right of the left side of the asteroid
								if (entities[i].getPosition().x > entities[j].getPosition().x - (entities[j].getGlobalBounds().width / 3.5))
								{
									//Checks if center is to the left of the right side of the asteroid
									if (entities[i].getPosition().x < entities[j].getPosition().x + (entities[j].getGlobalBounds().width / 3.5))
									{
										//Ship is colliding with asteroid
										if (playerLives > 0)
										{
											playerLives--;
											entities[i].shipReset();
										}
										else
										{
											std::cout << "You lost.\nYour final score: " << playerScore << std::endl;
											system("pause");
											exit(0);
										}
									}
								}
							}
						}
					}
				}

				//Check if bullet collides with asteroid
				if (i > 0 && i < 21)
				{
					for (int j = 21; j < 61; j++)
					{
						//Checks if bullet is below top of asteroid
						if (entities[i].getPosition().y > entities[j].getPosition().y - (entities[j].getGlobalBounds().height / 2))
						{
							//Checks if bullet is above bottom of asteroid
							if (entities[i].getPosition().y < entities[j].getPosition().y + (entities[j].getGlobalBounds().height / 2))
							{
								//Checks if bullet is to the right of the left side of the asteroid
								if (entities[i].getPosition().x > entities[j].getPosition().x - (entities[j].getGlobalBounds().width / 2))
								{
									//Checks if bullet is to the left of the right side of the asteroid
									if (entities[i].getPosition().x < entities[j].getPosition().x + (entities[j].getGlobalBounds().width / 2))
									{
										//Bullet is colliding with asteroid
										entities[j].resetAsteroid();
										entities[i].resetBullet();
										playerScore += 100;
									}
								}
							}
						}
					}
				}
			}

			//end Collision

			double currentVelocityX = entities[0].getVelocityX();
			double currentVelocityY = entities[0].getVelocityY();

			//Velocity change for ship
			if (upPressed)
			{

				//Alter VelocityX based off of current thrust
				entities[0].setVelocityX(currentVelocityX +
					std::cos((entities[0].getRotation() - 90) * M_PI / 180) *
					entities[0].getSpeed());

				//Alter VelocityY based off of current thrust
				entities[0].setVelocityY(currentVelocityY +
					std::sin((entities[0].getRotation() - 90) * M_PI / 180) *
					entities[0].getSpeed());
			}

			//Slow down ship if not thrusting and still moving
			else
			{
				//Slow down VelocityX if not equal to 0
				if (currentVelocityX != 0)
				{
					if (currentVelocityX < 0)
					{
						entities[0].setVelocityX((currentVelocityX)+entities[0].getFriction());
					}

					else
					{
						entities[0].setVelocityX((currentVelocityX)-entities[0].getFriction());
					}
				}

				//Slow down VelocityY if not equal to 0
				if (currentVelocityY != 0)
				{
					if (currentVelocityY < 0)
					{
						entities[0].setVelocityY((currentVelocityY)+entities[0].getFriction());
					}

					else
					{
						entities[0].setVelocityY((currentVelocityY)-entities[0].getFriction());
					}
				}
			}

			//Rotation for ship
			if (leftPressed)
			{
				entities[0].setRotation(entities[0].getRotation() - 2.5);
			}

			if (rightPressed)
			{
				entities[0].setRotation(entities[0].getRotation() + 2.5);
			}

			//Rotation for asteroids
			for (int i = 21; i < 41; i++)
			{
				if (!entities[i].getAvaliable())
				{
					entities[i].setRotation(entities[i].getRotation() + entities[i].getRotationSpeed());
				}
			}

			//Shoot bullets
			if (spacePressed)
			{
				int newBullet = requestBullet(entities);
				entities[newBullet].setAvaliable(false);
				entities[newBullet].setPosition(entities[0].getPosition());
				entities[newBullet].setRotation(entities[0].getRotation());

				entities[newBullet].setVelocityX(std::cos((entities[newBullet].getRotation() - 90) * M_PI / 180) *
					entities[newBullet].getSpeed());

				entities[newBullet].setVelocityY(std::sin((entities[newBullet].getRotation() - 90) * M_PI / 180) *
					entities[newBullet].getSpeed());

				spacePressed = false;
			}

			//Create asteroids
			while (asteroidTimer >= asteroidRate)
			{
				int newAsteroid = requestAsteroid(entities);
				entities[newAsteroid].setAvaliable(false);

				//Choose which side of the screen the asteroid will come from
				int randomNum = (rand() % 4) + 1;

				//Come from the left
				if (randomNum == 1)
				{
					randomNum = (rand() % windowHeight) + 1;
					entities[newAsteroid].setPosition(-100, randomNum);

					randomNum = (rand() % 90) + 1;
					entities[newAsteroid].setRotation(45 + randomNum);
				}
				//Come from the bottom
				else if (randomNum == 2)
				{
					randomNum = (rand() % windowWidth) + 1;
					entities[newAsteroid].setPosition(randomNum, windowHeight + 100);

					randomNum = (rand() % 90) + 1;
					entities[newAsteroid].setRotation(-45 + randomNum);
				}
				//Come from the right
				else if (randomNum == 3)
				{
					randomNum = (rand() % windowHeight) + 1;
					entities[newAsteroid].setPosition(windowWidth + 100, randomNum);

					randomNum = (rand() % 90) + 1;
					entities[newAsteroid].setRotation(-135 + randomNum);
				}
				//Come from the top
				else
				{
					randomNum = (rand() % windowWidth) + 1;
					entities[newAsteroid].setPosition(randomNum, -100);

					randomNum = (rand() % 90) + 1;
					entities[newAsteroid].setRotation(135 + randomNum);
				}

				entities[newAsteroid].setVelocityX(std::cos((entities[newAsteroid].getRotation() - 90) * M_PI / 180) *
					entities[newAsteroid].getSpeed());

				entities[newAsteroid].setVelocityY(std::sin((entities[newAsteroid].getRotation() - 90) * M_PI / 180) *
					entities[newAsteroid].getSpeed());

				asteroidTimer -= asteroidRate;
			}

			//Checks to see if invincibilty has worn out
			if (invincibilityTimer >= invincibilityLength)
			{
				entities[0].setInvincible(false);
				invincibilityTimer = sf::milliseconds(0);
			}

			//Adds flicker to invincibility effect
			if (!entities[0].getDestroyed())
			{
				if (entities[0].getInvincible() && flicker > 25)
				{
					if (entities[0].getColor() == sf::Color(255, 255, 255, 255))
					{
						entities[0].setColor(sf::Color(255, 255, 255, 75));
					}
					else
					{
						entities[0].setColor(sf::Color(255, 255, 255, 255));
					}
					flicker = 0;
				}
				else
				{
					flicker++;
				}
			}

			//Checks to see if ship should appear yet
			if (destroyedTimer >= destroyedLength)
			{
				entities[0].setDestroyed(false);
				destroyedTimer = sf::milliseconds(0);
			}

			//Changes entities locations based off VelocityX and VelocityY
			for (int i = 0; i < entities.size(); i++)
			{
				entities[i].setPosition(entities[i].getPosition().x + entities[i].getVelocityX(),
					entities[i].getPosition().y + entities[i].getVelocityY());
			}

			accumulator -= physicsRate;
		}

		//MAKE SHIP DISSAPEAR ON GAME OVER AND HAVE IT CONTINUE PLAYING FOR A FEW 
		//MOMENTS AT LEAST

		//Changes text for score and lives
		stream.str(std::string());
		stream << "LIVES|" << playerLives;

		lives.setString(stream.str());

		stream.str(std::string());
		stream << playerScore << "|Score";

		scoreText.setString(stream.str());

		//end Physics

		//end Logic

		//Render

		//Clears screen
		window.clear(sf::Color::Black);

		//Draws everything
		for (int i = 1; i < 21; i++)
		{
			window.draw(entities[i]);
		}

		window.draw(entities[0]);

		for (int i = 21; i < entities.size(); i++)
		{
			window.draw(entities[i]);
		}

		window.draw(lives);
		window.draw(scoreText);

		window.display();
		//end Render
	}

	return 0;
}

//THINGS TO DO:
//BUG: SHIP FLOATS DOWN AND TO THE LEFT WHEN STOPPED
// STATUS: DONE?
//ADD: VERSION CONTROL
// STATUS: DONE
//FIX: CLEAN UP SPAGHETTI CODE
//	STATUS: SHOULD EACH TYPE OF ENTITY HAVE ITS OWN CODE? YES, IT'D CLEAN THE CODE UP
//BUG: HITBOX DOESN'T SCALE WITH ASTEROID SIZE
//
//ADD: SMALLER ASTEROIDS SPAWN FROM DESTROYED ASTEROIDS
//
//ADD: GAME OVER SCREEN
//
//ADD: HIGH-SCORES
//
//ADD: ASTEROID EXPLOSIONS
//
//ADD: SHIP EXPLOSIONS
//
//ADD: PAUSE
//
//ADD: GAME TIMER
//
//ADD: PROGRESSIVE DIFFICULTY
//
//ADD: UI EFFECTS WHEN THINGS HAPPEN (+100 points) (Color Flashing)
//
//BUG: MAKE SCORE TEXT ADJUST TO WIDTH