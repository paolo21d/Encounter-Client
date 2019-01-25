#include "Communication.h"
#include "Location.h"
#include "Map.h"
#include "News.h"
#include "Game.h"


#include <SFML\Network.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace sf;

unsigned Location::areaSizeX = 50;
unsigned Location::areaSizeY = 25;

Communication::Communication() {
	//ip = IpAddress::getLocalAddress();
	ip = IpAddress("192.168.43.218"); //oneplus
	//ip = IpAddress("192.168.61.169"); //wieti wifi
	
	th = nullptr;
}

Communication::~Communication() {
}

void Communication::startCommunication(Game &g) {
	cout << "StartCommunication" << endl;
	Socket::Status status = socket.connect(ip, 2021);

	if (status != Socket::Done) {
		cout << "Cannot connect to server!" << endl;
		throw exception("-1");
	}
	//sleep(milliseconds(2000));
	//th = new thread(&Communication::srData, this);//dzialalo raczej
	//auto p = make_unique<thread>(&Communication::exploreCommunication, this, g);
	//th->join(); //to by zatrzymalo w tym miejscu wykonywanie
}

void Communication::receiveMap(Map &map) {
	cout << "Receive Map" << endl;
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
		file.open("../receiveImg/" + name, ios::out | ios::binary); //dorzuci� flage do nadpisywania
		//moze dorzucic czysczenie pliku
		file.write(buffer, buffSize);
		file.close();
	}*/
	//odbieram ramke: 1. mapSizeX 2. mapSizeY 3. ilosc lokacji 4. areaSizeX 5. areaSizeY 6. po kolei lokacje czyli najpierw id a potem src
	Packet packet2;
	socket.receive(packet2);
	cout << "Odebralem pakiet mapy" << endl;
	unsigned mapX, mapY, areaX, areaY, vecSize;
	string locationSrc;
	int locId;

	packet2 >> mapX;
	packet2 >> mapY;
	//Map tempMap(mapX, mapY);
	map.mapSizeX = mapX;
	map.mapSizeY = mapY;

	packet2 >> vecSize;
	packet2 >> areaX;
	packet2 >> areaY;
	Location::areaSizeX = areaX;
	Location::areaSizeY = areaY;

	int objId, objX, objY;
	string objSrc;
	int objSize; //ilosc obiektow wewnatrz danej lokacji
	//int objType; //typ objektu: 0-Dealer, 1-Nature, 2-Mob, 3-Chest
	for (int i = 0; i < vecSize; ++i) { //przesy�anie lokacji
		packet2 >> locId;
		packet2 >> locationSrc;
		Location tempLocation(locId, locationSrc);
		packet2 >> objSize;
		//wysy�anie co znajduje sie na lokacji czyli objectow
		for (int i = 0; i < objSize; ++i) {
			packet2 >> objId;
			packet2 >> objX;
			packet2 >> objY;
			packet2 >> objSrc;
			//packet2 >> objType;
			//Object* obj = new Object(objSrc, objId, objX, objY);
			//tempLocation.addObject(obj);
			tempLocation.addObject(new Object(objSrc, objId, objX, objY));
		}
		//map.addLocation(tempLocation);
		map.locations.push_back(tempLocation);
		map.locations[map.locations.size() - 1].setSprite();
		tempLocation.objects = {};
	}
	//przeslanie wszystkich kart
	int cardSize;
	int cardId, cardCostMana, cardDamage, cardCostGold;
	string cardSrc, cardName, cardDescription;
	//Card* card;
	packet2 >> cardSize; //przeslanie ile kart jest w sumie
	for (int i = 0; i < cardSize; ++i) {
		packet2 >> cardId;
		packet2 >> cardSrc;
		packet2 >> cardName;
		packet2 >> cardDescription;
		packet2 >> cardCostMana;
		packet2 >> cardDamage;
		packet2 >> cardCostGold;
		map.allCards.push_back(new Card(cardId, cardSrc, cardName, cardDescription, cardCostMana, cardDamage, cardCostGold));//tworzenie kart
		//card = new Card(cardId, cardSrc, cardName, cardDescription, cardCostMana, cardDamage, cardCostGold); 
		////map.addCard(card);
		//map.allCards.push_back(card);
	}
	//map = move(tempMap);
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

void Communication::sendReceiveData(Game &game) { //ta meteoda jest odpalana jako osobny w�tek - ona s�u�y do komunikacji
	NewsExplore nExploreRec;
	NewsDeal nDealRec;
	NewsFight nFightRec;
	while (1) {
		Packet receive, sent;
		//odbierz dane
		socket.receive(receive);
		if (game.mode == EXPLORE) {
			receive >> nExploreRec;
			mut.lock();
			game.mode = nExploreRec.gameMode;
			game.setMySquare(nExploreRec.positionX, nExploreRec.positionY);
			game.setOponentSquare(nExploreRec.oponentX, nExploreRec.oponentY, nExploreRec.oponentLocationId);
			for (int i = 0; i < 4; ++i)
				game.setAdjacent(i, nExploreRec.adjacent[i]);
			mut.unlock();
		}
		else if (game.mode == DEAL) {
			receive >> nDealRec;
			mut.lock();
			game.mode = nDealRec.gameMode;
			game.income = nDealRec.income;
			game.accpet = nDealRec.accept;
			for (unsigned i = 0; i < nDealRec.cardsId.size(); ++i) {
				for (unsigned f = 0; f < game.worldMap.allCards.size(); ++f) {
					if (nDealRec.cardsId[i] == game.worldMap.allCards[f]->id) { //dopasowane id karty
						game.cardsExchange.push_back(game.worldMap.allCards[f]);
					}
				}
			}
			game.mySquareX = nDealRec.areaToGoBackAfterDealX;
			game.mySquareY = nDealRec.areaToGoBackAfterDealY;
			game.myX = game.mySquareX * 50 + 25; //sprawdzic
			game.myY = game.mySquareY * 25 + 15; //sprawdzic

			mut.unlock();
		}
		else if (game.mode == FIGHT) {
			receive >> nFightRec;
		}

		//wysylaj dane
		if (game.mode == EXPLORE) {
			mut.lock();
			nExploreRec.positionX = game.mySquareX;
			nExploreRec.positionY = game.mySquareY;
			mut.unlock();
			sent << nExploreRec;
		}
		else if (game.mode == DEAL) {
			mut.lock();
			nDealRec.cardsId.clear();
			for (unsigned i = 0; i < game.pickedCards.size(); ++i)
				nDealRec.cardsId.push_back(game.pickedCards[i]->id);

			mut.unlock();
			sent << nDealRec;
		}
		else if (game.mode == FIGHT) {
			sent << nFightRec;
		}
		socket.send(sent);
	}
}

void Communication::startExploreCommunicationInOnotherThread(Game & game) {
	/*if(th!=nullptr)
		delete th;*/
	th = new thread(&Communication::exploreCommunication, this, ref(game));
	cout << "Odpalilem watek kominikacji explore" << endl;
	//auto p = make_unique<thread>(&Communication::exploreCommunication, this, ref(game));
	//thrUniquePtr = move(p);
}

void Communication::exploreCommunication(Game & game) { //Najpierw odbiera NewsExplore, nastepnie wysy�a NewsExplore, ustawia dane pola game 
	NewsExplore nExplore;
	Mode m;
	while (1) {
		cout << "KOMU KOMU" << endl;
		Packet receive, sent;
		//odbierz dane
		if (game.getMode() == EXPLORE) {
			cout << "Odbieram dane Explore" << endl;
			mut.lock();
			socket.receive(receive);
			mut.unlock();
			receive >> nExplore;
			cout << "odebralem dane Explore" << endl;
			game.setMode(nExplore.gameMode);
			game.setMySquare(nExplore.positionX, nExplore.positionY);
			game.setOponentSquare(nExplore.oponentX, nExplore.oponentY, nExplore.oponentLocationId);
			game.setEndGame(nExplore.endGame);
			for (int i = 0; i < 4; ++i)
				game.setAdjacent(i, nExplore.adjacent[i]);
			
			m = game.getMode();
			if (m == DEAL || m == FIGHT) { //rozpoczal sie tryb delaowania lub walki
				cout << "ZAMYKAM WATEK KOMUNIKACJI EXPLORE!!!" << endl;
				return; //zamykanie watku
			}

			/*if (game.getMode() == DEAL) { //zaczal sie tryb dealowania
				mut.unlock();
				return; //zamykanie watku
			} else if (game.getMode() == FIGHT) { //zaczal sie tryb walki
				mut.unlock();
				return; //zamykanie watku
			}*/
		}
		//mut.unlock();

		//wysylaj dane
		if (game.getMode() == EXPLORE) {
			nExplore.positionX = game.getMySquareX();
			nExplore.positionY = game.getMySquareY();
			//mut.unlock();
			sent << nExplore;
			cout << "Wysylam dane Explore" << endl;
			mut.lock();
			socket.send(sent);
			mut.unlock();
			cout << "Wyslalem dane Explore" << endl;
		}
	}
}

void Communication::exitCommunication() {
	//mut.lock();
	socket.disconnect();
	//mut.unlock();
}

void Communication::sendExploreNews(const NewsExplore & news) {
	Packet sent;
	sent << news;
	mut.lock();
	socket.send(sent);
	mut.unlock();
}

void Communication::sendDealNews(const NewsDeal & news) {
	Packet sent;
	sent << news;
	mut.lock();
	socket.send(sent);
	mut.unlock();
}

void Communication::sendFightNews(const NewsFight & news) {
	Packet sent;
	sent << news;
	mut.lock();	
	socket.send(sent);
	mut.unlock();
}

NewsExplore Communication::receiveExploreNews() {
	Packet get;
	mut.lock();
	socket.receive(get);
	mut.unlock();
	NewsExplore n;
	get >> n;
	return n;
}

NewsDeal Communication::receiveDealNews() {
	Packet get;
	mut.lock();
	socket.receive(get);
	mut.unlock();
	NewsDeal n;
	get >> n;
	return n;
}

NewsFight Communication::receiveFightNews() {
	Packet get;
	mut.lock();
	socket.receive(get);
	mut.unlock();
	NewsFight n;
	get >> n;
	return n;
}

sf::Packet Communication::receivePacket() {
	Packet get;
	mut.lock();
	socket.receive(get);
	mut.unlock();

	return get;
}
