#pragma once
#include "Location.h"
#include <vector>
class Map {
	std::vector <Location> locations;
	unsigned mapSizeX;
	unsigned mapSizeY;
public:
	Map();
	Map(unsigned x, unsigned y);
	~Map();
	void addLocation(Location & loc);
	//std::vector <Location>& getLocations();
};

