#include "Character.h"



Character::Character() {
	strength = 0;
	intelligence = 0;
	vitality = 0;
	gold = 0;
	hp = 0;
}


Character::~Character()
{
}

void Character::setSpriteHero(std::string src_) {
	srcName = src_;
	if (!texture.loadFromFile("../receiveImg/" + srcName))
		throw std::exception("-2");
	sprite.setTexture(texture);
}
