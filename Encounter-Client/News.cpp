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
	pckt << x.endGame;
	for (int i : {0, 1, 2, 3})
		pckt << x.adjacent[i];
	pckt << x.positionX << x.positionY;
	pckt << x.oponentLocationId << x.oponentX << x.oponentY;

	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsExplore & x) {
	int mode;
	pckt >> mode;
	x.gameMode = static_cast <Mode> (mode);
	pckt >> x.endGame;
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
	pckt >> x.oponentLocationId;
	pckt >> x.oponentX;
	pckt >> x.oponentY;
	return pckt;
}

sf::Packet & operator<<(sf::Packet & pckt, const NewsDeal & x) {
	pckt << x.gameMode;
	pckt << static_cast<int>(x.cardsId.size()); //ilosc kart do przeslania
	for (int i = 0; i < x.cardsId.size(); ++i)
		pckt << x.cardsId[i];
	pckt << x.areaToGoBackAfterDealX;
	pckt << x.areaToGoBackAfterDealY;
	pckt << x.accept;
	pckt << x.income;		
	pckt << x.addStrength;
	pckt << x.addIntelligence;
	pckt << x.addVitality;
	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsDeal & x) {
	int cardSize, mode, cardId;
	pckt >> mode;
	x.gameMode = static_cast<Mode>(mode);
	pckt >> x.accept;
	pckt >> x.areaToGoBackAfterDealX;
	pckt >> x.areaToGoBackAfterDealY;
	
	pckt >> cardSize; //ilosc kart do przeslania
	for (int i = 0; i < cardSize; ++i) {
		pckt >> cardId;
		x.cardsId.push_back(cardId);
	}
	
	pckt >> x.income;
	return pckt;
}

sf::Packet & operator<<(sf::Packet & pckt, const NewsFight & x) {
	/*pckt << x.endFight;
	pckt << x.strength[0] << x.strength[1];
	pckt << x.intelligence[0] << x.intelligence[1];
	pckt << x.vitality[0] << x.vitality[1];
	pckt << x.hp[0] << x.hp[1];
	pckt << x.mana[0] << x.mana[1];
	pckt << static_cast<int>(x.cardsId.size());
	for (int j = 0; j < x.cardAmount[0]; ++j)
		pckt << x.cardsId[j];*/
	pckt << x.chosenCard;

	return pckt;
}

sf::Packet & operator >> (sf::Packet & pckt, NewsFight & x) {
	pckt >> x.endFight;
	pckt >> x.strength[0];
	pckt >> x.strength[1];
	pckt >> x.intelligence[0];
	pckt >> x.intelligence[1];
	pckt >> x.vitality[0];
	pckt >> x.vitality[1];
	pckt >> x.hp[0];
	pckt >> x.hp[1];
	pckt >> x.mana[0];
	pckt >> x.mana[1];
	pckt >> x.cardAmount[0];
	pckt >> x.cardAmount[1];
	x.cardsId.clear();
	int xxx;
	for (int j = 0; j < x.cardAmount[0]; ++j) {
		pckt >> xxx;
		x.cardsId.push_back(xxx);
	}
	pckt >> x.chosenCard;

	return pckt;
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

NewsExplore::NewsExplore()
{
}

NewsDeal::NewsDeal()
{
}

NewsFight::NewsFight()
{
}
