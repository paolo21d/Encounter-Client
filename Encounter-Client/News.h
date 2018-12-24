#ifndef NEWS_H
#define NEWS_H
#include <iostream>
#include <fstream>
#include "SFML/Network.hpp"


enum Mode{EXPLORE, FIGHT, DEAL};

struct News{

	Mode gameMode;
	int firstFighterId;
	int secondFighterId;

	double dealerFactor;	
	
	int income;
	bool adjacent[4];			//0-góra, 1-prawa, 2-dół, 3-lewa
	int positionX, positionY;

	void reset();
	News();

	friend std::ostream& operator<<(std::ostream& os, const News& x);
	friend sf::Packet& operator<<(sf::Packet& pckt, const News& x);
	friend sf::Packet& operator>>(sf::Packet& pckt, News& x);
};

#endif
