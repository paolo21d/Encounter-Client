#include "Card.h"



Card::Card()
{
}

Card::Card(const int & id_, const std::string & src, const std::string & name_, const std::string & description_, const int & costMana_) {
	id = id_;
	srcName = src;
	name = name_;
	description = description_;
	costMana = costMana_;

	texture.loadFromFile("../receiveImg/" + srcName);
	sprite.setTexture(texture);
}

Card::~Card()
{
}
