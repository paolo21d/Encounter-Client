#pragma once
#include <SFML\Graphics.hpp>
class Location {
	int id;
	unsigned areaSizeX;
	unsigned areaSizeY;
	sf::Texture locationTexture;
	sf::Sprite locationSprite;
public:
	Location();
	Location(unsigned x, unsigned y, int id, std::string src);
	~Location();
};

