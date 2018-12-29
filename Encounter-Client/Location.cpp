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
	if (!locationTexture.loadFromFile("../receiveImg/" + src)) {
		throw exception("-1");
	}
	locationSprite.setTexture(locationTexture);
}


Location::~Location()
{
}

void Location::draw(sf::RenderWindow * appWindow) {
	Sprite sp;
	float x, y;
	for (auto it = objects.begin(); it != objects.end(); it++) {
		Object* obj = *it;
		if (obj->getVisibility() == false)
			continue;

		sp = obj->getSprite();
		x = obj->getX();
		y = obj->getY() - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
}

void Location::drawBackground(sf::RenderWindow * appWindow) {
	appWindow->draw(locationSprite);
}

int Location::getId() {
	return id;
}
