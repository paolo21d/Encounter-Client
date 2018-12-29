#pragma once
#include <string>
#include <SFML\Graphics.hpp>
class Card {
	int id;
	std::string srcName;
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Card();
	Card(int id_, std::string src);
	~Card();
};

