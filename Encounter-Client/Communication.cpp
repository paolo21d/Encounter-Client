#include "Communication.h"
#include "Location.h"
#include "Map.h"
#include "News.h"
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

void Communication::startCommunication() {
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
	Packet packet1;
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
	}
	//odbieram ramke: 1. mapSizeX 2. mapSizeY 3. ilosc lokacji 4. areaSizeX 5. areaSizeY 6. po kolei lokacje czyli najpierw id a potem src
	Packet packet2;
	socket.receive(packet2);
	unsigned mapX, mapY, areaX, areaY, vecSize;
	//Location tempLocation;
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
	for (int i = 0; i < vecSize; ++i) {
		packet2 >> locId;
		packet2 >> locationSrc;
		//dorzuciæ wysy³anie co znajduje sie na lokacji
		Location tempLocation(locId, locationSrc);
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
