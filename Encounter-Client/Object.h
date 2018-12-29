#pragma once
#include <SFML\Graphics.hpp>
class Object {
	std::string srcName;
	int id;
	sf::Texture texture;
	sf::Sprite sprite;
	int posX, posY;
	bool visibility;
public:
	Object() {
		//texture.loadFromFile("../receiveImg/" + srcName);
		visibility = true;
	}
	~Object();
	void setTexture();
	void setSprite();
	sf::Texture getTexture();
	sf::Sprite getSprite();
	int getX();
	int getY();
	bool getVisibility();
};

