#include "Communication.h"
#include "Location.h"
#include "Map.h"
#include <SFML\Network.hpp>
#include <iostream>
using namespace std;
using namespace sf;

Communication::Communication() {
	ip = IpAddress::getLocalAddress();
}


Communication::~Communication() {
}

void Communication::startCommunication() {
	Socket::Status status = socket.connect(ip, 2003);

	if (status != Socket::Done) {
		cout << "Cannot connect to server!" << endl;
		throw exception("-1");
	}
}

void Communication::receiveMap() {
	//odbieram grafiki i je zapisuje pod odpowiednia nazwa do folderu 'receiveImg'
	socket.receive(packet);

	//odbieram ramke: 1. mapSizeX 2. mapSizeY 3. ilosc lokacji 4. areaSizeX 5. areaSizeY 6. po kolei lokacje czyli najpierw id a potem src
	socket.receive(packet);
	unsigned x, y, areaX, areaY, vecSize;
	//Location tempLocation;
	string locationSrc;
	int locId;
	packet >> x;
	packet >> y;
	Map tempMap(x, y);
	packet >> vecSize;
	packet >> areaX;
	packet >> areaY;
	for (int i = 0; i < vecSize; ++i) {
		packet >> locId;
		packet >> locationSrc;
		Location tempLocation(areaX, areaY, locId, locationSrc);
		tempMap.addLocation(tempLocation);
	}
	
}
