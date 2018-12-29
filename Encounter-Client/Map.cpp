#include "Map.h"
#include "Card.h"

Map::Map() {
}

Map::Map(unsigned x, unsigned y) {
	mapSizeX = x;
	mapSizeY = y;
}

Map::~Map() {
}

void Map::addLocation(Location & loc) {
	locations.push_back(loc);
}

void Map::addCard(Card * card) {
	allCards.push_back(card);
}


