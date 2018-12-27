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
	if (!locationTexture.loadFromFile("../receiveImg/" + src)) {
		throw exception("-1");
	}
	locationSprite.setTexture(locationTexture);
}


Location::~Location()
{
}
