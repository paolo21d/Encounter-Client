#pragma once
#include "Map.h"
#include "News.h"
#include "Location.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

class Game {
	sf::RenderWindow *appWindow;
	Map worldMap;
	Location* currentLocation;
	//enum Mode { EXPLORE, FIGHT, DEAL };
	int adjacent[4]; //0-góra, 1-prawa, 2-dó³, 3-lewa 
					 //0-mozna wejsc, 1-pole nieinteraktywne, 2-pole interaktywne 
	int myX, myY; //wspolrzedne gracza
	int mySquareX, mySquareY; //wspolrzedne pola na ktorym sie znaduje
	int squareHeight, squareWidth; //wysokosc i szerokosc pól
public:
	Mode mode;

	Game();
	~Game();
	void intro();
	void enterName();
	void enterNameDraw(std::string &name, sf::Text &textName, sf::Text &hello);
	void setMap(Map& recMap);
	void cannnotConnect();

	void startGame();
	void explore();
	void fight();
	void deal();
};

