#pragma once
#include "Location.h"
#include "Card.h"
#include <vector>
class Map {
	std::vector <Location> locations;
	std::vector <Card*> allCards;
	unsigned mapSizeX;
	unsigned mapSizeY;
public:
	Map();
	Map(unsigned x, unsigned y);
	~Map();
	void addLocation(Location & loc);
	void addCard(Card *card);
	//std::vector <Location>& getLocations();
};

