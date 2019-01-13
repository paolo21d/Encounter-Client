#pragma once
#include <SFML\Graphics.hpp>
class Object {
protected:
	std::string srcName;
	int id;
	int posX, posY;

	bool visibility;
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Object() {
		//texture.loadFromFile("../receiveImg/" + srcName);
		visibility = true;
	}
	Object(std::string n, int id_, int posX_, int posY_);
	~Object();
	void setTexture();
	void setSprite();
	sf::Texture getTexture();
	sf::Sprite getSprite();
	int getX();
	int getY();
	void setVisibility(bool v);
	bool getVisibility();
};

