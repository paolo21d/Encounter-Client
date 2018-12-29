#ifndef NEWS_H
#define NEWS_H
#include <iostream>
#include <fstream>
#include "Character.h"
#include "Deck.h"
#include "SFML/Network.hpp"


enum Mode { EXPLORE, FIGHT, DEAL };

struct NewsExplore {
	//NewsExplore(Mode mode = EXPLORE, int posX = 0, int posY = 0);
	NewsExplore(Mode mode, int posX, int posY);
	NewsExplore();
	/*const Character* firstFighter; //przerzucić do klasy game
	const Character* secondFighter;*/
	//int income;
	Mode gameMode; //w obie strony
	int adjacent[4]; //0-góra, 1-prawa, 2-dół, 3-lewa //0-mozna wejsc, 1-pole nieinteraktywne, 2-pole interaktywne //wysylane z serwera do klienta
	int positionX, positionY; //wysylane od klienta do serwera
	int oponentLocationId;
	int oponentX, oponentY;

	friend std::ostream& operator<<(std::ostream& os, const NewsExplore& x);
	friend sf::Packet& operator<<(sf::Packet& pckt, const NewsExplore& x);
	friend sf::Packet& operator >> (sf::Packet& pckt, NewsExplore& x);
};

struct NewsDeal {
	NewsDeal();

	Mode gameMode;
	double dealerFactor;
	//Deck cardsForSale; //mozliwe ze zamienic na vector intow czyli id kart
	std::vector <int> cardsId;

	friend sf::Packet& operator<<(sf::Packet& pckt, const NewsDeal& x);
	friend sf::Packet& operator >> (sf::Packet& pckt, NewsDeal& x);
};

struct NewsFight {
	NewsFight();

	Mode gameMode;

	friend sf::Packet& operator<<(sf::Packet& pckt, const NewsFight& x);
	friend sf::Packet& operator >> (sf::Packet& pckt, NewsFight& x);
};
#endif
