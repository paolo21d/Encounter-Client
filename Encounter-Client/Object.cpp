#include "Object.h"



Object::Object(std::string n, int id_, int posX_, int posY_) {
	srcName = n;
	id = id_;
	posX = posX_;
	posY = posY_;
	texture.loadFromFile("../receiveImg/" + srcName);
	sprite.setTexture(texture);
}

Object::~Object()
{
}

void Object::setTexture() {
	texture.loadFromFile("../receiveImg/" + srcName);
}

void Object::setSprite() {
	setTexture();
	sprite.setTexture(texture);
}

sf::Texture Object::getTexture() {
	return texture;
}

sf::Sprite Object::getSprite() {
	return sprite;
}

int Object::getX() {
	return posX;
}

int Object::getY() {
	return posY;
}

void Object::setVisibility(bool v) {
	visibility = v;
}

bool Object::getVisibility() {
	return visibility;
}
