#include "Game.h"
#include "Communication.h"
#include "News.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <SFML\Window.hpp>

using namespace sf;
using namespace std;
/////////////////////////////////////moj z³oty kolor: 212, 175, 55
Game::Game(){
	appWindow = new RenderWindow(VideoMode(mapSizeX+infoWidth, mapSizeY, 32), "Encounter");
	appWindow->setFramerateLimit(60);
	mode = EXPLORE;
	endGame = 0;
	//font.loadFromFile("../img/Lato-Light.ttf");
	font.loadFromFile("../img/Charm-Regular.ttf");
	blockDealConnect = false;

	myHero = new Character;
	opponentHero = new Character;
	//squareHeight = 20;
	//squareHeight = 20;
	//RenderWindow temp(VideoMode(1114, 572, 32), "Encounter");
	//appWindow = temp;
}

Game::Game(Communication * comptr):Game() {
	communication = comptr;
}

Game::~Game(){
	delete appWindow;
	Location *loc;
	for (auto i = 0; i < worldMap.locations.size(); ++i) { //usuniecie objektow w lokacjach
		loc = &worldMap.locations[i];
		for (auto k = 0; k < loc->objects.size(); ++k)
			delete loc->objects[k];
	}

	for (auto i = 0; i < worldMap.allCards.size(); ++i) { //usuniecie wszystkich kart
		delete worldMap.allCards[i];
	}
}

void Game::intro() {
	string s;
	Texture tempTexture;
	Sprite tempSprite;
	cout << "Start" << endl;
	appWindow->clear(Color(255, 255, 255));
	tempTexture.loadFromFile("../img/intro.png");
	tempSprite.setTexture(tempTexture);
	tempSprite.setScale(Vector2f(0.2f, 0.2f));
	tempSprite.setPosition(sf::Vector2f(0, 0));
	appWindow->draw(tempSprite);
	appWindow->display();
	sleep(milliseconds(3000));

	for (int i = 0; i <= 23; ++i) {
		appWindow->clear(Color(255, 255, 255));
		cout << "Laduje " << i << endl;
		s = to_string(23 - i);
		tempTexture.loadFromFile("../img/" + s + ".png");
		tempSprite.setTexture(tempTexture);
		tempSprite.setScale(Vector2f(0.25f, 0.25f));
		tempSprite.setPosition(sf::Vector2f(50, 100));
		appWindow->draw(tempSprite);
		appWindow->display();
		sleep(milliseconds(50));
	}
	/*cout << "Mapa" << endl;
	Texture map;
	if (!map.loadFromFile("../img/mapka.png")) {
	cout << "Load Map Error" << endl;
	}
	Sprite mapSprite;
	mapSprite.setTexture(map);
	appWindow->draw(mapSprite);
	appWindow->display();
	sleep(milliseconds(1000));*/

}

void Game::enterName() {
	Text textName, hello;
	string name;
	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	background.setPosition(0, 0);

	setText(textName, 30, 212, 175, 60);
	textName.setPosition(200, 300);

	setText(hello, 40, 212, 175, 60);
	hello.setString(L"Witaj, wpisz swoje imiê  \nnastêpnie naciœnij enter aby rozpocz¹æ grê");
	hello.setPosition(100, 100);

	enterNameDraw(name, textName, hello, background);

	while (appWindow->isOpen()) {
		Event event;
		while (appWindow->pollEvent(event)) {
			if (event.type == Event::Closed) {
				appWindow->close();
				throw exception("0");
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				appWindow->close();
				throw exception("0");
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
				if (name.empty()) continue;
				//ustaw imie
				return;
			}
			else if (event.type == Event::KeyPressed && event.key.code >= Keyboard::A && event.key.code <= Keyboard::Z) {
				//cout << event.key.code + 65;
				cout << static_cast<char>(event.key.code + 65);
				name += static_cast<char>(event.key.code + 65);
				//name.append(event.key.code + 65);
			}
			else if(event.type == Event::KeyPressed && event.key.code == Keyboard::Backspace) {
				if (!name.empty()) {
					name.erase(name.end() - 1);
				}
			}

			enterNameDraw(name, textName, hello, background);
		}
	}
}

void Game::enterNameDraw(string &name, Text &textName, Text &hello, Sprite &background) {
	appWindow->clear(Color(150, 150, 150));
	appWindow->draw(background);
	appWindow->draw(hello);
	if (!name.empty()) {
		textName.setString(name);
		appWindow->draw(textName);
	}
	appWindow->display();
}

void Game::setMap(Map & recMap) {
	worldMap = recMap;
}

void Game::cannnotConnect() {
	Text text;	
	setText(text, 40, 212, 175, 55);
	text.setPosition(100, 400);
	text.setString(L"Nie mo¿na siê po³¹czyæ z serwerem!\nNaciœnij ESC");

	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	background.setPosition(0, 0);
	appWindow->clear(Color(50, 50, 50));
	appWindow->draw(background);
	appWindow->draw(text);
	appWindow->display();
	while (appWindow->isOpen()) {
		Event event;
		while (appWindow->pollEvent(event)) {
			if (event.type == Event::Closed) {
				appWindow->close();
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				appWindow->close();
			}
		}
	}
}

int Game::startGame() {
	//intro();
	try {
		enterName();
	} catch (...) {
		cout << "Zamykam" << endl;
		throw;
	}
	try {
		communication->startCommunication(*this);
		drawWaitingForOpponent();
	} catch (...) { //napisac komunikat ze nie mozna polaczyc a nie zamykac silowo
		cout << "Connection Error - cannot connect" << endl;
		cannnotConnect();
		throw;
	}
	try {
		communication->receiveMap(worldMap);
	} catch (...) {
		cout << "Cannot find file" << endl;
		throw;
	}
	cout << "Odbieram pakiet inicjalizacyjny" << endl;
	//odebranie packet inicjalizujacy
	int locIdMy, locIdOpp;
	Packet initPacket = communication->receivePacket();
	initPacket >> myHero->strength;
	initPacket >> myHero->intelligence;
	initPacket >> myHero->vitality;
	initPacket >> myHero->gold;
	initPacket >> locIdMy;
	currentLocation = &worldMap.locations[locIdMy];
	initPacket >> mySquareX;
	initPacket >> mySquareY;
	myX = mySquareX*squareWidth + 25;
	myY = mySquareY*squareHeight + 15;
	initPacket >> opponentLocationId;
	initPacket >> opponentSquareX;
	initPacket >> opponentSquareY;
	myHero->hp = myHero->vitality; //ma sie tyle hp ile ma sie witalnosci

	//odpalam watek komunikacji
	communication->startExploreCommunicationInOnotherThread(*this);
	explore(); //w³¹czenie trybu eksploracji
	communication->exitCommunication();
	return 0;
}

void Game::explore() {
	cout << "Zaczynam explore" << endl;
	Texture sideTexture;
	sideTexture.loadFromFile("../receiveImg/side_bar.png");
	Sprite sidebar;
	sidebar.setTexture(sideTexture);
	sidebar.setPosition(1000, 0);

	////staty
	Text tHp, tStat, tGold;
	setText(tHp, 20, 255, 51, 51);
	setText(tStat, 20, 255, 255, 255);
	setText(tGold, 20, 255, 255, 77); //moze byc color: 212, 175, 55
	tHp.setPosition(mapSizeX + 5, 50);
	tGold.setPosition(mapSizeX + 5, 100);
	tStat.setPosition(mapSizeX + 5, 150);
	////
	myHero->setSpriteHero("hero0.png");
	opponentHero->setSpriteHero("hero0.png");

	drawExplore(sidebar, tHp, tStat, tGold);

	while (appWindow->isOpen()) {
		Event event;
		if (mode == DEAL) { //wlaczamy tryb dealowania
			deal();
			communication->startExploreCommunicationInOnotherThread(*this);
		} else if (mode == FIGHT) { //wlaczamy tryb walki
			fight();
			mode = EXPLORE;
			NewsExplore nExp;
			nExp.gameMode = EXPLORE;
			nExp.positionX = mySquareX;
			nExp.positionY = mySquareY;
			communication->sendExploreNews(nExp); //taki pusty
			communication->startExploreCommunicationInOnotherThread(*this);
		}
		while (appWindow->pollEvent(event)) {
			if (event.type == Event::Closed) {
				appWindow->close();
				return;
			} 
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				appWindow->close();
				return;
			} 
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
				lock_guard<mutex> lock(protectData);
				if (myX - 1 <= 0)
					continue;
				if (adjacent[3] == 1) { //nieinteraktywne - nie mozna wejsc
					if((myX-1)/squareWidth != mySquareX)
						continue;
					myX--;
				} else if (adjacent[3] == 0) { //mozna wejsc na siasiadujace pole - puste
					myX--;
					if ((myX)/squareWidth != mySquareX) {//przeszedlem na sasiadujace pole
						mySquareX--;
					}
				} else { //interaktywne
					myX--;
					if ((myX)/squareWidth != mySquareX) {//wszed³em na pole interaktywne
						mySquareX--;
						//ale nie zmieniam myX ?!?!?!?!
					}
				}
			} 
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
				lock_guard<mutex> lock(protectData);
				if (myX + 1 >= mapSizeX)
					continue;
				if (adjacent[1] == 1) { //nieinteraktywne
					if ((myX + 1)/squareWidth != mySquareX)
						continue;
					myX++;
				} else if (adjacent[1] == 0) { //mozna wejsc na siasiadujace pole - puste
					myX++;
					if ((myX)/squareWidth != mySquareX) {//przeszedlem na sasiadujace pole
						mySquareX++;
					}
				} else { //interaktywne
					myX++;
					if ((myX)/squareWidth != mySquareX) {//wszed³em na pole interaktywne
						mySquareX++;
					}
				}
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) {
				lock_guard<mutex> lock(protectData);
				if (myY - 1 <= 0)
					continue;
				if (adjacent[0] == 1) { //nieinteraktywne
					if ((myY - 1) / squareHeight != mySquareY)
						continue;
					myY--;
				} else if (adjacent[0] == 0) { //mozna wejsc na siasiadujace pole - puste
					myY--;
					if (myY / squareHeight != mySquareY) {
						mySquareY--;
					}
				} else { //interaktywne
					myY--;
					if (myY / squareHeight != mySquareY) {
						mySquareY--;
					}
				}
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down) {
				lock_guard<mutex> lock(protectData);
				if (myY + 1 >= mapSizeY)
					continue;
				if (adjacent[2] == 1) { //nieinteraktywne
					if ((myY + 1) / squareHeight != mySquareY)
						continue;
					myY++;
				} else if (adjacent[2] == 0) { //mozna wejsc na siasiadujace pole - puste
					myY++;
					if (myY / squareHeight != mySquareY) {
						mySquareY++;
					}
				} else { //interaktywne
					myY++;
					if (myY / squareHeight != mySquareY) {
						mySquareY++;
					}
				}
			}
			protectData.lock(); ///nie wiem czy nie przerzuciæ tych 3 linii za t¹ klamrê od while event
			drawExplore(sidebar, tHp, tStat, tGold);
			protectData.unlock();
		}//while obslugi eventow
		protectData.lock();
		if (endGame != 0) { //konczymy gre
			string infoEndGame;
			if (endGame == 1) { //przegralem w walce
				//ekran koñca gry
				infoEndGame = "Koniec gry. Przegra³eœ...";
			} else if (endGame == 2) { //wygralem z przeciwnikiem
				//ekran koñca gry	
				infoEndGame = "Wygra³eœ! Przeciwnik zosta³ pokonany";
				//zaznaczenie ¿e ju¿ odwiedzi³em mobka wiêc nie bêdzie wyœwietlany
				Object *obj;
				for (auto it = currentLocation->objects.begin(); it != currentLocation->objects.end(); it++) {
					obj = *it;
					if (obj->getX() == mySquareX && obj->getY()) { //znalaz³em obiekt mobka, zmieniam jego visibility na false
						obj->setVisibility(false);
					}
				}
			} else if (endGame == 3) { //przeciwnik siê roz³¹czy³
				//ekran koñca gry
				infoEndGame = "Koniec gry. Przeciwnik roz³¹czy³ siê...";
			}
			drawEndGame(infoEndGame);
			while (appWindow->isOpen()) {
				Event e;
				while (appWindow->pollEvent(e)) {
					if (e.type == Event::Closed) {
						appWindow->close();
						return;
					} else if (e.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
						appWindow->close();
						return;
					}
				}
			}
		}
		protectData.unlock();
	}
}

void Game::fight() {
	cout << "Zaczynam Fight!!!!" << endl;
	bool clickedCard = false;
	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	background.setPosition(0, 0);
	Card* opponentPickedCard = nullptr;
	Text opponentMove;
	setText(opponentMove, 30, 212, 175, 55);
	opponentMove.setString(L"Czekam na ruch przeciwnika...");
	opponentMove.setPosition(350, 250);

	NewsFight news;
	news.gameMode = FIGHT;
	news.endFight = -1;
	communication->sendFightNews(news); //potwierdzenie trybu fight od klienta
	///odebrac fight news inicjalizuj¹cy
	cout << "Probuje odebrac info inicajali" << endl;
	news = communication->receiveFightNews(); //odebranie ini
	cout << "Odebralem info ini" << endl;
	setCardsOnHand(news);
	int mobIndexInLocationArray = news.vitality[0];
	int mobIndexInObjectArray = news.vitality[1];

	news.vitality[0] = news.hp[0];
	news.vitality[1] = news.hp[1];

	drawFightHideOpponentCard(background, news);//narysowanie
	
	news = communication->receiveFightNews(); //odebranie juz pierwszego konkretnego
	setCardsOnHand(news);
	if (news.chosenCard != -1) { //zaczynal przeciwnik czyli wyswietl jego karte
		drawFightHideOpponentCard(background, news);
		for (auto i = 0; i < worldMap.allCards.size(); ++i) { //wybrana karta przez oponenta
			if (news.chosenCard == worldMap.allCards[i]->id)
				opponentPickedCard = worldMap.allCards[i];
		}
		Sprite sp = opponentPickedCard->sprite;
		sp.setPosition(400, 200);
		appWindow->draw(sp);
		appWindow->display();
		this_thread::sleep_for(chrono::milliseconds(1000));
		if (news.endFight != 0) { //koniec walki, ktos wygral juz teraz
			mode = EXPLORE;
			worldMap.locations[mobIndexInLocationArray].objects[mobIndexInObjectArray]->setVisibility(false);
			return;
		}
	} else { //ja zaczynam | chosenCard==-1
		drawFightHideOpponentCard(background, news);
	}

	while (appWindow->isOpen()) {
		Event event;
		while (appWindow->pollEvent(event)) {
			if (event.type == Event::Closed) {
				appWindow->close();
				throw exception("0");
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				appWindow->close();
				throw exception("0");
			}
			else if (event.type == Event::MouseButtonPressed) {
				Vector2i mousePosition = Mouse::getPosition(*appWindow);
				int mouseX = mousePosition.x;
				int mouseY = mousePosition.y;
				if (mouseY >= 325 && mouseY <= 525) { //pasek moich kart
					if (mouseX >= 175 && mouseX <= 325 && myCardsOnHand.size() >= 1) { //1. karta
						if (news.mana[0] < myCardsOnHand[0]->costMana) continue;
						news.chosenCard = myCardsOnHand[0]->id;
						clickedCard = true;
					} else if (mouseX >= 350 && mouseX <= 500 && myCardsOnHand.size() >= 2) { //2. karta
						if (news.mana[0] < myCardsOnHand[1]->costMana) continue;
						news.chosenCard = myCardsOnHand[1]->id;
						clickedCard = true;
					} else if (mouseX >= 525 && mouseX <= 675 && myCardsOnHand.size() >= 3) { //3. karta
						if (news.mana[0] < myCardsOnHand[2]->costMana) continue;
						news.chosenCard = myCardsOnHand[2]->id;
						clickedCard = true;
					} else if (mouseX >= 700 && mouseX <= 850 && myCardsOnHand.size() >= 4) { //4. karta
						if (news.mana[0] < myCardsOnHand[3]->costMana) continue;
						news.chosenCard = myCardsOnHand[3]->id;
						clickedCard = true;
					} else if (mouseX >= 875 && mouseX <= 1025 && myCardsOnHand.size() >= 5) { //5. karta
						if (news.mana[0] < myCardsOnHand[4]->costMana) continue;
						news.chosenCard = myCardsOnHand[4]->id;
						clickedCard = true;
					}
				}
			}
		}//while lapanie eventow
		if (clickedCard == true) { //wybrano karte
			clickedCard = false;
			communication->sendFightNews(news); //wyslanie jaka karte wybralem
			news = communication->receiveFightNews(); //odebranie reakcji po moim zagraniu
			if (news.endFight != 0) { //zakonczona walka
				//1-ja wygralem; 2-wygral przeciwnik
				mode = EXPLORE;
				worldMap.locations[mobIndexInLocationArray].objects[mobIndexInObjectArray]->setVisibility(false);
				return;
			}
			setCardsOnHand(news);
			//myHero->hp = news.hp[0];
			
			//myHero->
			drawFightHideOpponentCard(background, news);
			appWindow->draw(opponentMove);
			appWindow->display();
			news = communication->receiveFightNews(); //odebranie info o ruchu przeciwnika
			if (news.endFight != 0) { //koniec walki
				mode = EXPLORE;
				worldMap.locations[mobIndexInLocationArray].objects[mobIndexInObjectArray]->setVisibility(false);
				return;
			}
			for (auto i = 0; i < worldMap.allCards.size(); ++i) { //wybrana karta przez oponenta
				if (news.chosenCard == worldMap.allCards[i]->id)
					opponentPickedCard = worldMap.allCards[i];
			}
			//wyswietlic przez chwile jaka karte wybral oponent
			Sprite sp = opponentPickedCard->sprite;
			sp.setPosition(400, 200);
			appWindow->draw(sp);
			appWindow->display();
			sleep(milliseconds(1000));
			drawFightHideOpponentCard(background, news);//narysowanie
		}
	}
}

void Game::deal() {
	cout << "Zaczynam deal!!" << endl;
	bool selectedCard[5];
	for (unsigned i = 0; i < 5; ++i) selectedCard[i] = false;
	unsigned addStrength = 0, addIntelligence = 0, addVitality = 0;
	int currentGold = static_cast<int>(myHero->gold);

	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	background.setPosition(0, 0);

	NewsDeal news;
	news.gameMode = DEAL;
	communication->sendDealNews(news);
	news = communication->receiveDealNews(); //odebranie calego info o kartach itp
	mode = news.gameMode;
	income = news.income;
	accpet = news.accept;
	cardsExchange.clear();
	for (unsigned i = 0; i < news.cardsId.size(); ++i) {
		for (unsigned f = 0; f < worldMap.allCards.size(); ++f) {
			if (news.cardsId[i] == worldMap.allCards[f]->id) { //dopasowane id karty
				cardsExchange.push_back(worldMap.allCards[f]);
			}
		}
	}
	//mySquareX = news.areaToGoBackAfterDealX;
	//mySquareY = news.areaToGoBackAfterDealY;
	//myX = mySquareX * squareWidth + 25; //sprawdzic
	//myY = mySquareY * squareHeight + 15; //sprawdzic


	if (income == 0) { //deal with Dealer
		drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
		while (appWindow->isOpen()) {
			Event event;
			while (appWindow->pollEvent(event)) {
				if (event.type == Event::Closed) {
					appWindow->close();
					throw exception("0");
				} else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
					appWindow->close();
					throw exception("0");
				} else if (event.type == Event::MouseButtonPressed) {
					//sprawdzic w jakie miejsce klikniete !!!!
					Vector2i mousePosition = Mouse::getPosition(*appWindow);
					int mouseX = mousePosition.x;
					int mouseY = mousePosition.y;
					cout << "Mouse X: " << mouseX << ", Y: " << mouseY << endl;
					if (mouseY >= 25 && mouseY <= 225) { //pasek grafik kart
						if (mouseX >= 50 && mouseX <= 200 && cardsExchange.size() >= 1) { //1. karta
							if (selectedCard[0] == false && currentGold - cardsExchange[0]->costGold >= 0) { //zaznaczam karte
								selectedCard[0] = true;
								currentGold -= cardsExchange[0]->costGold;
							} else if (selectedCard[0] == true) { //odznaczam karte
								selectedCard[0] = false;
								currentGold += cardsExchange[0]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 250 && mouseX <= 400 && cardsExchange.size() >= 2) { //2. karta
							if (selectedCard[1] == false && currentGold - cardsExchange[1]->costGold >= 0) { //zaznaczam karte
								selectedCard[1] = true;
								currentGold -= cardsExchange[1]->costGold;
							} else if (selectedCard[1] == true) { //odznaczam karte
								selectedCard[1] = false;
								currentGold += cardsExchange[1]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 450 && mouseX <= 600 && cardsExchange.size() >= 3) { //3. karta
							if (selectedCard[2] == false && currentGold - cardsExchange[2]->costGold >= 0) { //zaznaczam karte
								selectedCard[2] = true;
								currentGold -= cardsExchange[2]->costGold;
							} else if (selectedCard[2] == true) { //odznaczam karte
								selectedCard[2] = false;
								currentGold += cardsExchange[2]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						} 
						else if (mouseX >= 650 && mouseX <= 800 && cardsExchange.size() >= 4) { //4. karta
							if (selectedCard[3] == false && currentGold - cardsExchange[3]->costGold >= 0) { //zaznaczam karte
								selectedCard[3] = true;
								currentGold -= cardsExchange[3]->costGold;
							} else if (selectedCard[3] == true) { //odznaczam karte
								selectedCard[3] = false;
								currentGold += cardsExchange[3]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 850 && mouseX <= 1000 && cardsExchange.size() >= 5) { //5. karta
							if (selectedCard[4] == false && currentGold - cardsExchange[4]->costGold >= 0) { //zaznaczam karte
								selectedCard[4] = true;
								currentGold -= cardsExchange[4]->costGold;
							} else if (selectedCard[4] == true) { //odznaczam karte
								selectedCard[4] = false;
								currentGold += cardsExchange[4]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
					}
					else if (mouseY >= 365 && mouseY <= 440) { //pasek statystyk
						if (mouseX >= 125 && mouseX <= 175) { //zwieksz sile
							if (currentGold - costStrength >= 0) {
								currentGold -= costStrength;
								addStrength++;
								drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
							}
						}
						else if (mouseX >= 425 && mouseX <= 475) { //zwieksz inteligencje
							if (currentGold - costIntelligence >= 0) {
								currentGold -= costIntelligence;
								addIntelligence++;
								drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
							}
						}
						else if (mouseX >= 725 && mouseX <= 775) { //zwieksz vitality
							if (currentGold - costVitality >= 0) {
								currentGold -= costVitality;
								addVitality++;
								drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
							}
						}
					}
					else if (mouseY >= 480 && mouseY <= 520) { //pasek przyscisku accept
						if (mouseX >= 800 && mouseX <= 950) { //przycisk akceptuj
							news.gameMode = DEAL;
							news.cardsId.clear();
							for (auto i = 0; i < 5; ++i) {
								if (selectedCard[i] == true) {
									news.cardsId.push_back(cardsExchange[i]->id);
								}
							}
							news.addStrength = addStrength;
							news.addIntelligence = addIntelligence;
							news.addVitality = addVitality;
							communication->sendDealNews(news); //wyslanie info z tym co chce zakupic
							//mozna dopisac ewentualnie jakies wysiwtlenie ze udalo sie zakupic te rzeczy
							news = communication->receiveDealNews(); //pole news.accept

							//ustaw moje zloto i staty
							myHero->gold = currentGold;
							myHero->strength += addStrength;
							myHero->intelligence += addIntelligence;
							myHero->vitality += addVitality;
							myHero->hp = myHero->vitality;

							mySquareX = news.areaToGoBackAfterDealX;
							mySquareY = news.areaToGoBackAfterDealY;
							myX = mySquareX * squareWidth + 25;
							myY = mySquareY * squareHeight + 15;

							mode = EXPLORE;
							NewsExplore nExp;
							nExp.gameMode = EXPLORE;
							nExp.positionX = mySquareX;
							nExp.positionY = mySquareY;
							communication->sendExploreNews(nExp);
							return; //konczymy transakcje
						}
					}

				}
			}
		}
	}
	else { //deal with Chest
		drawDealChest(selectedCard, currentGold, background); //rysuje tylko raz ekran dealowania z chest bo nic tam sie nie zmienia
		//zaznaczenie ¿e ju¿ odwiedzona skrzynka wiêc nie bêdzie wyœwietlana
		Object *obj;
		for (auto it = currentLocation->objects.begin(); it != currentLocation->objects.end(); it++) {
			obj = *it;
			if (obj->getX() == mySquareX && obj->getY() == mySquareY) { //znalaz³em obiekt skrzynki, zmieniam jej visibility na false
				obj->setVisibility(false);
			}
		}
		//dodaj z³oto mi
		myHero->gold += income;

		while (appWindow->isOpen()) {
			Event event;
			//drawDealChest(selectedCard, currentGold, background);
			while (appWindow->pollEvent(event)) {
				if (event.type == Event::Closed) {
					appWindow->close();
					throw exception("0");
				}
				else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
					appWindow->close();
					throw exception("0");
				}
				else if (event.type == Event::MouseButtonPressed) {
					//sprawdzic w jakie miejsce klikniete !!!!
					Vector2i mousePosition = Mouse::getPosition(*appWindow);
					int mouseX = mousePosition.x;
					int mouseY = mousePosition.y;
					if (mouseY >= 480 && mouseY <= 520) { //pasek przycisku accept
						if (mouseX >= 800 && mouseX <= 950) { //przycisk akceptuj
							news.gameMode = DEAL;
							news.cardsId.clear();
							for (auto i = 0; i < 5; ++i) {
								news.cardsId.push_back(cardsExchange[i]->id);
							}
							communication->sendDealNews(news);
							//mozna dopisac ewentualnie jakies wysiwtlenie ze udalo sie zakupic te rzeczy
							news = communication->receiveDealNews(); //pole news.accept
							mySquareX = news.areaToGoBackAfterDealX;
							mySquareY = news.areaToGoBackAfterDealY;
							myX = mySquareX * squareWidth + 25;
							myY = mySquareY * squareHeight + 15; 
							//cout << "MySX" << mySquareX << " MySY" << mySquareY << endl;
							mode = EXPLORE;
							NewsExplore nExp;
							nExp.gameMode = EXPLORE;
							nExp.positionX = mySquareX;
							nExp.positionY = mySquareY;
							communication->sendExploreNews(nExp);
							return; //konczymy transakcje
						}
					}

				}
			}
			//drawDealChest(selectedCard, currentGold, background);
		}
	}
	
}

void Game::drawExplore(Sprite &sidebar, Text &tHp, Text &tStat, Text &tGold) {
	//cout << "Rysuje mape" << endl;
	appWindow->clear(Color(150, 150, 150));
	//currentLocation->drawBackground(appWindow);
	appWindow->draw(currentLocation->locationSprite);
	appWindow->draw(sidebar);
	Sprite sp;
	int x, y;
	bool paintedHero = false;
	bool paintedOpponent = false;
	
	RectangleShape objSquare(sf::Vector2f(100, 50));
	objSquare.setFillColor(Color(150, 150, 150, 50));
	RectangleShape heroSquare(sf::Vector2f(100, 50));
	heroSquare.setFillColor(Color(255, 150, 150, 50));
	//objSquare.setPosition(800, 480);

	for(auto i=0; i<currentLocation->objects.size(); ++i){
		Object *obj = currentLocation->objects[i];
		if (obj->getVisibility() == false)//sprawdzenie czy mamy rysowaæ dany obiekt, czy tez zostal juz odwiedzony i jest niewidzialny
			continue;
		if (obj->getY() <= mySquareY && !paintedHero) { //rysuj mojego bohatera w odpowiednim miejscu
			objSquare.setPosition(obj->getX()*squareWidth, obj->getY()*squareHeight);
			appWindow->draw(objSquare);
			sp = obj->getSprite();
			x = obj->getX() * squareWidth;
			y = (obj->getY() + 1) * squareHeight - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
			////
			heroSquare.setPosition(mySquareX*squareWidth, mySquareY*squareHeight);
			appWindow->draw(heroSquare);
			sp = myHero->getSprite();
			x = myX;
			y = myY - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
			paintedHero = true;
			continue;
		}
		if (opponentLocationId == currentLocation->getId() && obj->getY() <= opponentSquareY && !paintedOpponent) { //rysuj bohatera przeciwnka w opowiednim miejscu
			objSquare.setPosition(obj->getX()*squareWidth, obj->getY()*squareHeight);
			appWindow->draw(objSquare);
			sp = obj->getSprite();
			x = obj->getX() * squareWidth + squareWidth/2;
			y = (obj->getY() + 1) * squareHeight - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
			///
			heroSquare.setPosition(opponentSquareX*squareWidth, opponentSquareY*squareHeight);
			appWindow->draw(heroSquare);
			sp = opponentHero->getSprite();
			x = opponentSquareX * squareWidth;
			y = (opponentSquareY) * squareHeight - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
			paintedOpponent = true;
			continue;
		}
		objSquare.setPosition(obj->getX()*squareWidth, obj->getY()*squareHeight);
		appWindow->draw(objSquare);
		sp = obj->getSprite();
		x = obj->getX() * squareWidth;
		y = (obj->getY()+1) * squareHeight - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	if (!paintedHero) { //rysuj mojego bohatera na samym dole mapy
		heroSquare.setPosition(mySquareX*squareWidth, mySquareY*squareHeight);
		appWindow->draw(heroSquare);
		//
		sp = myHero->getSprite();
		x = myX;
		y = myY - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	if (opponentLocationId == currentLocation->getId() && !paintedOpponent) { //rysuj bohatera przeciwnika na dole mapy
		heroSquare.setPosition(opponentSquareX*squareWidth, opponentSquareY*squareHeight);
		appWindow->draw(heroSquare);
		//	
		sp = opponentHero->getSprite();
		x = opponentSquareX * squareWidth + squareWidth/2;
		y = (opponentSquareY + 1) * squareHeight - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	//rysowanie paska info
	
	tHp.setString("HP:\n" + to_string(myHero->hp));
	tGold.setString(L"\nGold:\n" + to_string(myHero->gold));
	tStat.setString(L"\n\nStr:\n" + to_string(myHero->strength) + L"\nInt:\n" + to_string(myHero->intelligence) + L"\nVit:\n" + to_string(myHero->vitality));

	appWindow->draw(tHp);
	appWindow->draw(tGold);
	appWindow->draw(tStat);

	appWindow->display();
}

void Game::drawFight(Sprite &background) {
	appWindow->clear(Color(153, 51, 51));
	appWindow->draw(background);
	Sprite cardBack;
}

void Game::drawFightHideOpponentCard(sf::Sprite & background, const NewsFight &news) {
	appWindow->clear(Color(153, 51, 51));
	appWindow->draw(background);
	Sprite cardBack, myCard;
	Texture cb;
	cb.loadFromFile("../receiveImg/reverse.png");
	cardBack.setTexture(cb);
	//rysowanie awatarow - napisac
	/*Sprite avatar1, avatar2; //avatar1 - opponent, avatar2 - myHero
	Texture tav1, tav2; //zaladowac obrazki z pliku
	avatar1.setTexture(tav1);
	avatar2.setTexture(tav2);
	avatar1.setPosition(25, 25);
	avatar2.setPosition(25, 375);
	appWindow->draw(avatar1);
	appWindow->draw(avatar2);*/
	//rysowanie odwrotow kart przeciwnika
	for (int i = 0; i < news.cardAmount[1]; ++i) {
		cardBack.setPosition(175 + i * 175, 25);
		appWindow->draw(cardBack);
	}
	//rysowanie moich kart
	for (int i = 0; i < myCardsOnHand.size(); ++i) {
		myCard = myCardsOnHand[i]->sprite;
		myCard.setPosition(175 + i * 175, 325);
		appWindow->draw(myCard);
	}
	//narysowanie statysyk (HP)
	Text myHp, opHp, myMana;
	setText(myHp, 25, 212, 175, 55);
	setText(opHp, 25, 212, 175, 55);
	//setText(myMana, 25, 212, 175, 55);
	myHp.setString("HP: " + to_string(news.hp[0])+"\nMANA: " + to_string(news.mana[0]));
	opHp.setString("HP: " + to_string(news.hp[1]));

	myHp.setPosition(25, 350);
	opHp.setPosition(25, 175);
	appWindow->draw(myHp);
	appWindow->draw(opHp);
	
	Text myStat, opStat;
	setText(myStat, 12, 212, 175, 55);
	setText(opStat, 12, 212, 175, 55);
	myStat.setString(L"Str: " + to_string(news.strength[0]) + L"\nInt: " + to_string(news.intelligence[0]) + L"\nVit: " + to_string(news.vitality[0]));
	opStat.setString(L"Str: " + to_string(news.strength[1]) + L"\nInt: " + to_string(news.intelligence[1]) + L"\nVit: " + to_string(news.vitality[1]));
	myStat.setPosition(25, 300); //zmienic te wymiary!!!
	opStat.setPosition(25, 230); //sprawdzic wymiary!!!!
	appWindow->draw(myStat);
	appWindow->draw(opStat);

	//narysowanie info co ma zrobic gracz
	Text info;
	setText(info, 25, 212, 175, 55);

	appWindow->display();
}

void Game::drawFightShowOpponentCard(sf::Sprite & background, const NewsFight &news) {
	appWindow->clear(Color(153, 51, 51));
	appWindow->draw(background);
	Sprite cardBack;
}

void Game::drawFightEnd(sf::Sprite & background, const NewsFight & news) {
	appWindow->draw(background);
	Text t;
	setText(t, 40, 212, 175, 55);
	t.setPosition(350, 240);
	if (news.endFight == 1) {
		t.setString(L"Wygra³eœ!");
	} else if (news.endFight == 2) {
		t.setString(L"Zosta³eœ pokonany...\nSpróbuj ponownie.");
	}
}

void Game::drawDealDealer(const bool *selectedCards, const unsigned &addStrength, const unsigned &addIntelligence, const unsigned &addVitality, const unsigned &currentGold, Sprite &background) {
	appWindow->clear(Color(153, 51, 51));
	appWindow->draw(background);
	Sprite sp;
	RectangleShape backCard(sf::Vector2f(160, 210));
	backCard.setFillColor(Color(255, 223, 0));
	for (int i = 0; i < cardsExchange.size(); ++i) { //rysywanie kart
		sp = cardsExchange[i]->sprite;
		sp.setPosition(Vector2f(200 * i + 50, 25));
		if (selectedCards[i]) { //wybrana-zaznaczona = rysuj 'obramowanie'
			backCard.setPosition(200 * i + 45, 20);
			appWindow->draw(backCard);
		}
		appWindow->draw(sp);
	}
	/////// staty kart
	Text tDamage, tCostGold, tCostMana;
	setText(tDamage, 20, 255, 204, 204);
	setText(tCostGold, 20, 255, 255, 153); //moze byc color: 212, 175, 55
	setText(tCostMana, 20, 153, 204, 255);
	for (auto i = 0; i < cardsExchange.size(); ++i) { //rysuj staty karty pod ni¹
		tDamage.setPosition(50 + 200 * i, 245);
		tCostGold.setPosition(50 + 200 * i, 275);
		tCostMana.setPosition(50 + 200 * i, 305);
		tDamage.setString(L"Obra¿enia: " + to_string(cardsExchange[i]->damage));
		tCostGold.setString(L"Koszt z³ota: " + to_string(cardsExchange[i]->costGold));
		tCostMana.setString("Koszt many: " + to_string(cardsExchange[i]->costMana));
		appWindow->draw(tDamage);
		appWindow->draw(tCostGold);
		appWindow->draw(tCostMana);
	}
	/////// zakup statow
	Text statName, statPoints, statCost, statPlus;
	setText(statName, 30, 255, 255, 255);
	setText(statPoints, 50, 255, 255, 255);
	setText(statCost, 20, 255, 255, 255);
	setText(statPlus, 50, 255, 255, 255);
	statPlus.setString("+");
	RectangleShape showStatRec(sf::Vector2f(50, 75));
	showStatRec.setFillColor(Color(153, 153, 102));
	RectangleShape addStat(sf::Vector2f(50, 75));
	addStat.setFillColor(Color(153, 153, 102));
	//add strength
	showStatRec.setPosition(50, 365);
	statPoints.setString(to_string(myHero->strength + addStrength));
	statPoints.setPosition(60, 377);
	appWindow->draw(showStatRec);
	appWindow->draw(statPoints);
	addStat.setPosition(125, 365);
	statPlus.setPosition(150, 377);
	appWindow->draw(addStat);
	appWindow->draw(statPlus);
	statName.setString(L"Si³a");
	statName.setPosition(185, 377);
	appWindow->draw(statName);
	statCost.setString(to_string(costStrength));
	statCost.setPosition(185, 435);
	appWindow->draw(statCost);
	//add intelligence
	showStatRec.setPosition(350, 365);
	statPoints.setString(to_string(myHero->intelligence + addIntelligence));
	statPoints.setPosition(360, 377);
	appWindow->draw(showStatRec);
	appWindow->draw(statPoints);
	addStat.setPosition(425, 365);
	statPlus.setPosition(450, 377);
	appWindow->draw(addStat);
	appWindow->draw(statPlus);
	statName.setString(L"Inteligencja");
	statName.setPosition(485, 377);
	appWindow->draw(statName);
	statCost.setString(to_string(costIntelligence));
	statCost.setPosition(485, 435);
	appWindow->draw(statCost);
	//add vitality
	showStatRec.setPosition(650, 365);
	statPoints.setString(to_string(myHero->vitality + addVitality));
	statPoints.setPosition(660, 377);
	appWindow->draw(showStatRec);
	appWindow->draw(statPoints);
	addStat.setPosition(725, 365);
	statPlus.setPosition(750, 377);
	appWindow->draw(addStat);
	appWindow->draw(statPlus);
	statName.setString(L"Witalnoœæ");
	statName.setPosition(785, 377);
	appWindow->draw(statName);
	statCost.setString(to_string(costVitality));
	statCost.setPosition(785, 435);
	appWindow->draw(statCost);
	/////// wyswietl ilosc golda
	Text gold;
	setText(gold, 50, 255, 255, 255);
	gold.setString(L"Masz z³ota: " + to_string(currentGold));
	gold.setPosition(450, 480);
	appWindow->draw(gold);
	/////// wyswietl przycisk accept
	RectangleShape accpetButton(sf::Vector2f(150, 40));
	accpetButton.setFillColor(Color(38, 13, 13));
	accpetButton.setPosition(800, 480);
	Text accept;
	setText(accept, 30, 255, 255, 255);
	accept.setPosition(850, 485);
	accept.setString("Akceptuj");
	appWindow->draw(accpetButton);
	appWindow->draw(accept);

	appWindow->display();
}

void Game::drawDealChest(const bool * selectedCards, const unsigned & currentGold, Sprite &background) {
	appWindow->clear(Color(153, 51, 51));
	appWindow->draw(background);
	Sprite sp;
	RectangleShape backCard(sf::Vector2f(160, 210));
	backCard.setFillColor(Color(255, 223, 0));
	for (int i = 0; i < cardsExchange.size(); ++i) { //rysywanie kart
		sp = cardsExchange[i]->sprite;
		sp.setPosition(Vector2f(200 * i + 50, 25));
		//if (selectedCards[i]) { //wybrana-zaznaczona = rysuj 'obramowanie'
			backCard.setPosition(200 * i + 45, 20);
			appWindow->draw(backCard);
		//}
		appWindow->draw(sp);
	}
	/////// staty kart
	Text tDamage, tCostGold, tCostMana;
	tDamage.setFont(font);
	tCostGold.setFont(font);
	tCostMana.setFont(font);
	tDamage.setFillColor(Color(255, 204, 204));
	tCostGold.setFillColor(Color(255, 255, 153));
	tCostMana.setFillColor(Color(153, 204, 255));
	tDamage.setCharacterSize(20);
	tCostGold.setCharacterSize(20);
	tCostMana.setCharacterSize(20);
	for (auto i = 0; i < cardsExchange.size(); ++i) { //rysuj staty karty pod ni¹
		tDamage.setPosition(50 + 200 * i, 245);
		tCostGold.setPosition(50 + 200 * i, 275);
		tCostMana.setPosition(50 + 200 * i, 305);
		tDamage.setString(L"Obra¿enia: " + to_string(cardsExchange[i]->damage));
		tCostGold.setString(L"Koszt z³ota: " + to_string(cardsExchange[i]->costGold));
		tCostMana.setString("Koszt many: " + to_string(cardsExchange[i]->costMana));
		appWindow->draw(tDamage);
		appWindow->draw(tCostGold);
		appWindow->draw(tCostMana);
	}
	//wyswitl ile kasy dostajesz ze skrzynki
	Text tIncome;
	tIncome.setFont(font);
	tIncome.setFillColor(Color(255, 255, 255));
	tIncome.setCharacterSize(50);
	tIncome.setString(L"Dostajesz z³ota: " + to_string(income));
	tIncome.setPosition(350, 400);
	appWindow->draw(tIncome);
	/////// wyswietl ilosc golda
	Text gold;
	gold.setFont(font);
	gold.setFillColor(Color(255, 255, 255));
	gold.setCharacterSize(50);
	gold.setString(L"Masz z³ota: " + to_string(myHero->gold));
	gold.setPosition(450, 480);
	appWindow->draw(gold);
	/////// wyswietl przycisk accept
	RectangleShape acceptButton(sf::Vector2f(150, 40));
	acceptButton.setFillColor(Color(38, 13, 13));
	acceptButton.setPosition(800, 480);
	Text accept;
	setText(accept, 30, 212, 175, 55);
	accept.setPosition(825, 480);
	accept.setString("Akceptuj");
	appWindow->draw(acceptButton);
	appWindow->draw(accept);
	
	appWindow->display();
}

void Game::drawEndGame(string info) {
	Text t;
	setText(t, 50, 212, 175, 55);
	t.setString(info);
	t.setPosition(100, 200);
	appWindow->draw(t);
	appWindow->display();
}

void Game::drawWaitingForOpponent() {
	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	background.setPosition(0, 0);
	appWindow->draw(background);

	Text info;
	setText(info, 50, 212, 175, 55);
	info.setString(L"Czekam na po³¹czenie przeciwnika...");
	info.setPosition(200, 200);
	appWindow->draw(info);
	appWindow->display();
}

void Game::setMySquare(const int & x, const int & y) {
	lock_guard<mutex> lock(protectData);
	if (mySquareX != x) {//niezgodnosc
		//myX = x*squareWidth + 25;
	}
	if (mySquareY != y) {//niezgodnosc
		//myY = y*squareHeight + 10;
	}
	//mySquareX = x;
	//mySquareY = y;
}

void Game::setOponentSquare(const int & x, const int & y, const int & loc) {
	lock_guard<mutex> lock(protectData);
	opponentSquareX = x;
	opponentSquareY = y;
	opponentLocationId = loc;
}

void Game::setAdjacent(int index, int val){
	lock_guard<mutex> lock(protectData);
	adjacent[index] = val;
}

void Game::setMode(Mode mode_) {
	lock_guard<mutex> lock(protectData);
	mode = mode_;
}

Mode Game::getMode() {
	lock_guard<mutex> lock(protectData);
	Mode mode_ = mode;
	return mode_;
}

int Game::getMySquareX() {
	lock_guard<mutex> lock(protectData);
	int sx = mySquareX;
	return sx;
}

int Game::getMySquareY() {
	lock_guard<mutex> lock(protectData);
	int sy = mySquareY;
	return sy;
}

inline void Game::setText(sf::Text & text, const int & fontSize, const int & r, const int & g, const int & b) {
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(Color(r, g, b));
}

inline void Game::setCardsOnHand(const NewsFight & news) {
	myCardsOnHand.clear();
	for (auto i = 0; i < news.cardsId.size(); ++i) { //wrzucenie kart ktore ja mam do myCardsOnHand
		for (unsigned f = 0; f < worldMap.allCards.size(); ++f) {
			if (news.cardsId[i] == worldMap.allCards[f]->id) { //dopasowane id karty
				myCardsOnHand.push_back(worldMap.allCards[f]);
			}
		}
	}
}


