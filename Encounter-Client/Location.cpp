#include "Location.h"
#include <SFML\Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

Location::Location() {
}

Location::Location(int locid, string src) {
	/*areaSizeX = x;
	areaSizeY = y;*/
	id = locid;
	srcTexture = src;
	/*if (!locationTexture.loadFromFile("../receiveImg/" + src)) {
		throw exception("-1");
	}
	locationSprite.setTexture(locationTexture);
	locationSprite.setPosition(0, 0);*/
}

Location::~Location() {
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete *it;
	}
}


/*void Location::draw(sf::RenderWindow * appWindow) {
	Sprite sp;
	float x, y;
	for (auto it = objects.begin(); it != objects.end(); it++) {
		Object* obj = *it;
		//if (obj->getVisibility() == false)
		//	continue;

		sp = obj->getSprite();
		x = obj->getX();
		y = obj->getY() - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
}*/

void Location::addObject(Object * obj) {
	objects.push_back(obj);
}

void Location::drawBackground(sf::RenderWindow * appWindow) {
	appWindow->draw(locationSprite);
}

void Location::setSprite() {
	if (!locationTexture.loadFromFile("../receiveImg/" + srcTexture)) {
		throw exception("-1");
	}
	locationSprite.setTexture(locationTexture);
	locationSprite.setPosition(0, 0);
}

int Location::getId() {
	return id;
}
