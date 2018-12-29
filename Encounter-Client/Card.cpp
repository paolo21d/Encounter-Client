#include "Card.h"



Card::Card()
{
}

Card::Card(int id_, std::string src) {
	id = id_;
	srcName = src;
	texture.loadFromFile("../receiveImg/" + srcName);
	sprite.setTexture(texture);
}


Card::~Card()
{
}
