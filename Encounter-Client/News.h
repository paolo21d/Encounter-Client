#ifndef NEWS_H
#define NEWS_H
#include <iostream>
#include <vector>
#include <fstream>
//#include "Deck.h"
#include "SFML/Network.hpp"


enum Mode { EXPLORE, FIGHT, DEAL };

struct NewsExplore {
	//NewsExplore(Mode mode = EXPLORE, int posX = 0, int posY = 0);
	NewsExplore(Mode mode, int posX, int posY);
	NewsExplore();

	Mode gameMode; //w obie strony
	int endGame; //0-trwa gra, 1-ja przegtralem (umarlem w walce), 2-ja wygralem(pokonalem przeciwnika), 3-przeciwnik uciekl(rozlaczyl sie)
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
	bool accept;
	int areaToGoBackAfterDealX, areaToGoBackAfterDealY;
	std::vector<int>cardsId;
	int income; //==0 -na polu Dealer; >0 -na polu Chest
	int addStrength, addIntelligence, addVitality;

	friend sf::Packet& operator<<(sf::Packet& pckt, const NewsDeal& x);
	friend sf::Packet& operator >> (sf::Packet& pckt, NewsDeal& x);
};

struct NewsFight {
	// [0] - moje; [1] - przeciwnika
	int endFight; //0-konutunuujemy walke, 1-ja wygralem, 2-przeciwnik wygral
	//bool myTurn; //1- moja tura, 0-tura przeciwnika
	int strength[2];
	int intelligence[2];
	int vitality[2];
	int hp[2];
	int mana[2];
	int cardAmount[2]; //ilosc kart na stole obu kazdego z gracza
	std::vector<int> cardsId;	// posiadane karty
	int chosenCard; //klinet->server wysyła id karty ktora wybral; server->klient wysyła id karty ktora zagral przeciwnik

	NewsFight();

	Mode gameMode;
	
	friend sf::Packet& operator<< (sf::Packet& pckt, const NewsFight& x);
	friend sf::Packet& operator>> (sf::Packet& pckt, NewsFight& x);
};
#endif
