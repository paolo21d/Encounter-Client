#pragma once
#include <string>
#include <SFML\Graphics.hpp>
class Card {
	int id;
	std::string srcName; //kazda karta ma miec grafike 40x80

	std::string name;
	std::string description;
	int costMana;
	int damage; //dodane
	int costGold; //dodane

	sf::Texture texture;
	sf::Sprite sprite;
public:
	Card();
	Card(const int &id_, const std::string &src, const std::string &name_, const std::string &description_, const int &costMana_, const int &damage_, const int &costGold_);
	~Card();

	friend class Communication;
	friend class Game;
};

