#include "News.h"

using namespace std;

ostream& operator<<(ostream& os, const NewsExplore& x) {
	if (x.gameMode == FIGHT)
		os << "FIGHT " /*<<  x.firstFighter->getId() << " " << x.secondFighter->getId()*/ << endl;
	else if (x.gameMode == DEAL)
		os << "TRADE " <</* x.dealerFactor <<*/ endl;
	else
		os << "EXPLO" << endl;
	cout << "sądziedzi: ";
	for (int i : {0, 1, 2, 3}) cout << " " << x.adjacent[i];
	cout << endl/* << "zarobiłem: " << x.income << endl*/ << "pozycja: " << x.positionX << x.positionY << endl << endl;

	return os;
}

sf::Packet& operator<<(sf::Packet& pckt, const NewsExplore& x) {
	pckt << x.gameMode;
	for (int i : {0, 1, 2, 3}) pckt << x.adjacent[i];
	pckt << x.positionX << x.positionY;

	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsExplore & x)
{
	int mode;
	pckt >> mode;
	x.gameMode = static_cast <Mode> (mode);
	/*if (mode == 0) {
	x.gameMode = EXPLORE;
	} else if (mode == 1) {
	x.gameMode = FIGHT;
	} else if (mode == 2) {
	x.gameMode = DEAL;
	}*/
	for (int i : {0, 1, 2, 3}) pckt >> x.adjacent[i];
	pckt >> x.positionX;
	pckt >> x.positionY;
	return pckt;
}

sf::Packet & operator<<(sf::Packet & pckt, const NewsDeal & x)
{
	// TODO: insert return statement here
	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsDeal & x)
{
	// TODO: insert return statement here
	return pckt;
}

sf::Packet & operator<<(sf::Packet & pckt, const NewsFight & x)
{
	// TODO: insert return statement here
	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsFight & x)
{
	// TODO: insert return statement here
	return pckt;
}


NewsExplore::NewsExplore() {
	gameMode = EXPLORE;
	for (int i : {1, 2, 3, 4}) {
		adjacent[i] = 0;
	}
	//positionX = positionY = 0;
	positionX = 0;
	positionY = 0;
}

NewsExplore::NewsExplore(Mode mode = EXPLORE, int posX = 0, int posY = 0) {
	gameMode = mode;
	for (int i : {1, 2, 3, 4}) {
		adjacent[i] = 0;
	}
	//positionX = positionY = 0;
	positionX = posX;
	positionY = posY;
}
