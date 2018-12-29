#include "Object.h"



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

bool Object::getVisibility() {
	return visibility;
}
