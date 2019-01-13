#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Object.h"
class Location {
	int id;
	std::string srcTexture;

	sf::Texture locationTexture;
	sf::Sprite locationSprite;
	
public:
	std::vector <Object*> objects;

	static unsigned areaSizeX;
	static unsigned areaSizeY;
	Location();
	Location(int id, std::string src);
	~Location();

	//void draw(sf::RenderWindow *appWindow);
	void addObject(Object *obj);
	void drawBackground(sf::RenderWindow *appWindow);
	void setSprite();
	int getId();

	friend class Game;
};

