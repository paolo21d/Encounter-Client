#include "Map.h"
#include "Card.h"

Map::Map() {
}

Map::Map(unsigned x, unsigned y) {
	mapSizeX = x;
	mapSizeY = y;
}

Map::~Map() {
	for (unsigned int i = 0; i < allCards.size(); ++i) {
		delete allCards[i];
	}
}

void Map::addLocation(Location & loc) {
	locations.push_back(loc);
}

void Map::addCard(Card * card) {
	allCards.push_back(card);
}


