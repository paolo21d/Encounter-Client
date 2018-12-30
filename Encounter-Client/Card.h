#pragma once
#include <string>
#include <SFML\Graphics.hpp>
class Card {
	int id;
	std::string srcName;

	std::string name;
	std::string description;
	int costMana;

	sf::Texture texture;
	sf::Sprite sprite;
public:
	Card();
	Card(const int &id_, const std::string &src, const std::string &name_, const std::string &description_, const int &costMana_);
	~Card();
};

