#include "News.h"

using namespace std;

ostream& operator<<(ostream& os, const News& x) {
	if(x.gameMode == 1)
		os << "FIGHT " <<  x.firstFighterId << " " << x.secondFighterId << endl;
	else if(x.gameMode == 2)
		os << "TRADE " << x.dealerFactor << endl;
	else if(x.gameMode == 0)
		os << "EXPLO" << endl;
	cout << "sądziedzi: ";
	for(int i: {0, 1, 2, 3}) cout << " " << x.adjacent[i];
	cout << endl << "zarobiłem: " << x.income << endl << "pozycja: " << x.positionX << x.positionY << endl << endl;
	
	return os;
}

sf::Packet& operator<<(sf::Packet& pckt, const News& x){
	pckt << x.gameMode;
	for(int i: {0, 1, 2, 3}) pckt << x.adjacent[i];
	pckt << x.positionX << x.positionY;

	return pckt;
}

sf::Packet& operator>>(sf::Packet& pckt, News& x){
	int temp;
	pckt >> temp;
 	x.gameMode = static_cast<Mode>(temp);
	for(int i: {0, 1, 2, 3}) pckt >> x.adjacent[i];
	pckt >> x.positionX >> x.positionY;

	return pckt;
}

News::News(){
	gameMode = EXPLORE;
	firstFighterId = -1;
	secondFighterId = -1;
	dealerFactor = 0;	
	income = 0;
	for(int i:{1,2,3,4}){
		adjacent[i] = 0;
	}
	positionX = positionY = 0;
}

void News::reset(){
	gameMode = EXPLORE;
	firstFighterId = -1;
	secondFighterId = -1;

	dealerFactor = 0;	
	
	income = 0;
}
