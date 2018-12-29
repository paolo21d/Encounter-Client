#pragma once
class Card {
	int id;
	std::string srcImage;
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Card();
	~Card();
};

