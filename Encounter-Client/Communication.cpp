#include "Communication.h"
#include "Location.h"
#include "Map.h"
#include "News.h"
#include "Game.h"

#include "Nature.h"
#include "Chest.h"
#include "Mob.h"

#include <SFML\Network.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace sf;

unsigned Location::areaSizeX = 0;
unsigned Location::areaSizeY = 0;

Communication::Communication() {
	ip = IpAddress::getLocalAddress();
}


Communication::~Communication() {
}

void Communication::startCommunication(Game &g) {
	Socket::Status status = socket.connect(ip, 2003);

	if (status != Socket::Done) {
		cout << "Cannot connect to server!" << endl;
		throw exception("-1");
	}
	//sleep(milliseconds(2000));
	th = new thread(&Communication::srData, this);
	//th->join();
}

void Communication::receiveMap() {
	//odbieram grafiki i je zapisuje pod odpowiednia nazwa do folderu 'receiveImg'
	//odbieram ramke z informacja ile grafik otrzymam
	/*Packet packet1;
	socket.receive(packet1);
	unsigned GraphicsQuantity;
	packet1 >> GraphicsQuantity;
	string name;
	const unsigned buffSize = 70000; //przetestowac rozmiar i dopasowac do najwiekszej grafiki
	char buffer[buffSize]; 
	fstream file;
	for (unsigned i = 0; i < GraphicsQuantity; ++i) { //odbieram po kolei ramki z grafikami w postaci textu i ich nazwy, zapisuje je od folderu 'receiveImg'
		Packet packet;
		socket.receive(packet);
		packet >> name;
		packet >> buffer;
		file.open("../receiveImg/" + name, ios::out | ios::binary); //dorzuciæ flage do nadpisywania
		//moze dorzucic czysczenie pliku
		file.write(buffer, buffSize);
		file.close();
	}*/
	//odbieram ramke: 1. mapSizeX 2. mapSizeY 3. ilosc lokacji 4. areaSizeX 5. areaSizeY 6. po kolei lokacje czyli najpierw id a potem src
	Packet packet2;
	socket.receive(packet2);
	unsigned mapX, mapY, areaX, areaY, vecSize;
	string locationSrc;
	int locId;

	packet2 >> mapX;
	packet2 >> mapY;
	Map tempMap(mapX, mapY);

	packet2 >> vecSize;
	packet2 >> areaX;
	packet2 >> areaY;
	Location::areaSizeX = areaX;
	Location::areaSizeY = areaY;

	int objId, objX, objY;
	string objSrc;
	int objSize; //ilosc obiektow wewnatrz danej lokacji
	int objType; //typ objektu: 0-Dealer, 1-Nature, 2-Mob, 3-Chest
	for (int i = 0; i < vecSize; ++i) { //przesy³anie lokacji
		packet2 >> locId;
		packet2 >> locationSrc;
		Location tempLocation(locId, locationSrc);
		packet2 >> objSize;
		//wysy³anie co znajduje sie na lokacji czyli objectow
		for (int i = 0; i < objSize; ++i) {
			packet2 >> objId;
			packet2 >> objX;
			packet2 >> objY;
			packet2 >> objSrc;
			packet2 >> objType;
			Object* obj = new Object(objSrc, objId, objX, objY);
			tempLocation.addObject(obj);
			/*if (objType == 0) { //dealer
				//pisac
			}
			else if (objType == 1) { //Nature
				Object* obj = new Nature(); //pisac
			}
			else if (objType == 2) { //Mob
				//pisac
			}
			else if (objType == 3) { //Chest

			}*/
		}
		tempMap.addLocation(tempLocation);
	}
	
}

void Communication::srData() {
	string msgClient, msgServer;
	int i = 0;
	while (1) {
		Packet receivePacket, sentPacket;
		sleep(milliseconds(2000));
		socket.receive(receivePacket);
		receivePacket >> msgServer;
		cout << msgServer << endl;
		//sleep(milliseconds(2000));
		msgClient = "Klient msg " + to_string(i);
		sentPacket << msgClient;
		socket.send(sentPacket);
		++i;
	}
}

void Communication::sendReceiveData(Game &game)
{
	NewsExplore nExploreRec;
	NewsDeal nDealRec;
	NewsFight nFightRec;
	while (1) {
		Packet receive, sent;
		socket.receive(receive);
		if (game.mode == EXPLORE) {
			receive >> nExploreRec;
		}
		else if (game.mode == DEAL) {
			receive >> nDealRec;
		}
		else if (game.mode == FIGHT) {
			receive >> nFightRec;
		}

		//wysylaj dane
		if (game.mode == EXPLORE) {
			sent << nExploreRec;
		}
		else if (game.mode == DEAL) {
			sent << nDealRec;
		}
		else if (game.mode == FIGHT) {
			sent << nFightRec;
		}
		socket.send(sent);
	}
}
