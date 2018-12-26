#pragma once
#include "Map.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

class Game {
	sf::RenderWindow *appWindow;
	Map worldMap;
public:
	Game();
	~Game();
	void intro();
	void setMap(Map& recMap);
};

