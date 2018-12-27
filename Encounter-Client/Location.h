#pragma once
#include <SFML\Graphics.hpp>
#include "Object.h"
class Location {
	int id;
	sf::Texture locationTexture;
	sf::Sprite locationSprite;
	std::vector <Object*> objects;
public:
	static unsigned areaSizeX;
	static unsigned areaSizeY;
	Location();
	Location(int id, std::string src);
	~Location();
};

