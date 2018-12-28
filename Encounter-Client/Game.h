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
	void enterName();
	void enterNameDraw(std::string &name, sf::Text &textName, sf::Text &hello);
	void setMap(Map& recMap);
	void cannnotConnect();

	void startGame();
};

