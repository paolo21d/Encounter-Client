#include "Map.h"


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


