#include "Game.h"
#include "Communication.h"
#include "News.h"
#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <SFML\Window.hpp>

using namespace sf;
using namespace std;

Game::Game(){
	appWindow = new RenderWindow(VideoMode(mapSizeX+infoWidth, mapSizeY, 32), "Encounter");
	mode = EXPLORE;
	//font.loadFromFile("../img/Lato-Light.ttf");
	font.loadFromFile("../img/Charm-Regular.ttf");
	blockDealConnect = false;
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

void Game::startGame() {

}

void Game::explore() {
	Texture sideTexture;
	sideTexture.loadFromFile("../receiveImg/side_bar.png");
	Sprite sidebar;
	sidebar.setTexture(sideTexture);
	sidebar.setPosition(1000, 0);

	drawExplore(sidebar);

	while (appWindow->isOpen()) {
		Event event;
		if (mode == DEAL) { //wlaczamy tryb delaowania
			//mutBlockCommunication.lock();
			deal();
			communication->startExploreCommunicationInOnotherThread(*this);
			//mutBlockCommunication.unlock();
		} else if (mode == FIGHT) { //wlaczamy tryb walki
			//mutBlockCommunication.lock();
			fight();
			communication->startExploreCommunicationInOnotherThread(*this);
			//mutBlockCommunication.unlock();
		}
		while (appWindow->pollEvent(event)) {
			if (event.type == Event::Closed) {
				appWindow->close();
				throw exception("0");
			} else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				appWindow->close();
				throw exception("0");
			} 
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
				if (adjacent[3] == 1) { //nieinteraktywne
					if((myX-1)*squareWidth / squareWidth != mySquareX)
						continue;
					myX--;
				} else if (adjacent[3] == 0) { //mozna wejsc na siasiadujace pole - puste
					myX--;
					//sprawdzenie czy sie przeszlo juz na sasiednie pole
				} else { //interaktywne

				}
			} 
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
				if (adjacent[1] == 1) { //nieinteraktywne
					if ((myX + 1)*squareWidth / squareWidth != mySquareX)
						continue;
					myX++;
				} else if (adjacent[1] == 0) { //mozna wejsc na siasiadujace pole - puste
					myX++;
					//sprawdzenie czy sie przeszlo juz na sasiednie pole
				} else { //interaktywne

				}
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) {
				if (adjacent[0] == 1) { //nieinteraktywne
					if ((myY - 1)*squareHeight / squareHeight != mySquareY)
						continue;
					myY--;
				} else if (adjacent[0] == 0) { //mozna wejsc na siasiadujace pole - puste
					myY--;
					//sprawdzenie czy przeszlo sie na sasiednie pole
				} else { //interaktywne

				}
			}
			else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down) {
				if (adjacent[2] == 1) { //nieinteraktywne
					if ((myY + 1)*squareHeight / squareHeight != mySquareY)
						continue;
					myY++;
				} else if (adjacent[2] == 0) { //mozna wejsc na siasiadujace pole - puste
					myY++;
					//sprawdzenie czy przeszlo sie na sasiednie pole
				} else { //interaktywne

				}
			}

			drawExplore(sidebar);
		}
	}
}

void Game::fight() {
	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);
	
	NewsFight news;
	news.gameMode = FIGHT;
	communication->sendFightNews(news); //potwierdzenie trybu fight od klienta
	//odebrac fight news
	news = communication->receiveFightNews();
	myCardsOnHand.clear();
	for (auto i = 0; i < news.cardAmount[0]; ++i) { //wrzucenie kart ktore ja mam do myCardsOnHand
		for (unsigned f = 0; f < worldMap.allCards.size(); ++f) {
			if (news.cardsId[0][i] == worldMap.allCards[f]->id) { //dopasowane id karty
				myCardsOnHand.push_back(worldMap.allCards[f]);
			}
		}
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

					} else if (mouseX >= 350 && mouseX <= 500 && myCardsOnHand.size() >= 2) { //2. karta

					} else if (mouseX >= 525 && mouseX <= 675 && myCardsOnHand.size() >= 3) { //3. karta

					} else if (mouseX >= 700 && mouseX <= 850 && myCardsOnHand.size() >= 4) { //4. karta

					} else if (mouseX >= 875 && mouseX <= 1025 && myCardsOnHand.size() >= 5) { //5. karta

					}
				}
			}
		}
	}
}

void Game::deal() {
	bool selectedCard[5];
	for (unsigned i = 0; i < 5; ++i) selectedCard[i] = false;
	unsigned addStrength = 0, addIntelligence = 0, addVitality = 0;
	unsigned currentGold = myHero->gold;

	Texture back;
	back.loadFromFile("../receiveImg/background.png");
	Sprite background;
	background.setTexture(back);

	NewsDeal news;
	news.gameMode = DEAL;
	communication->sendDealNews(news);
	news = communication->receiveDealNews(); //odebranie calego info o kartach itp
	mode = news.gameMode;
	income = news.income;
	accpet = news.accept;
	for (unsigned i = 0; i < news.cardsId.size(); ++i) {
		for (unsigned f = 0; f < worldMap.allCards.size(); ++f) {
			if (news.cardsId[i] == worldMap.allCards[f]->id) { //dopasowane id karty
				cardsExchange.push_back(worldMap.allCards[f]);
			}
		}
	}
	mySquareX = news.areaToGoBackAfterDealX;
	mySquareY = news.areaToGoBackAfterDealY;
	myX = mySquareX * 50 + 25; //sprawdzic
	myY = mySquareY * 25 + 15; //sprawdzic


	if (income == 0) { //deal with Dealer
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
					//sprawdzic w jakie miejsce klikniete !!!!
					Vector2i mousePosition = Mouse::getPosition(*appWindow);
					int mouseX = mousePosition.x;
					int mouseY = mousePosition.y;
					if (mouseY >= 25 && mouseY <= 225) { //pasek grafik kart
						if (mouseX >= 50 && mouseX <= 200 && cardsExchange.size() >= 1) { //1. karta
							if (selectedCard[0] == false && currentGold - cardsExchange[0]->costGold >= 0) { //zaznaczam karte
								selectedCard[0] = true;
								currentGold -= cardsExchange[0]->costGold;
							}
							else if (selectedCard[0] == false) { //odznaczam karte
								selectedCard[0] = false;
								currentGold += cardsExchange[0]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 250 && mouseY <= 400 && cardsExchange.size() >= 2) { //2. karta
							if (selectedCard[1] == false && currentGold - cardsExchange[1]->costGold >= 0) { //zaznaczam karte
								selectedCard[1] = true;
								currentGold -= cardsExchange[1]->costGold;
							}
							else if (selectedCard[1] == false) { //odznaczam karte
								selectedCard[1] = false;
								currentGold += cardsExchange[1]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 450 && mouseY <= 600 && cardsExchange.size() >= 3) { //3. karta
							if (selectedCard[2] == false && currentGold - cardsExchange[2]->costGold >= 0) { //zaznaczam karte
								selectedCard[2] = true;
								currentGold -= cardsExchange[2]->costGold;
							}
							else if (selectedCard[2] == false) { //odznaczam karte
								selectedCard[2] = false;
								currentGold += cardsExchange[2]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 650 && mouseY <= 800 && cardsExchange.size() >= 4) { //4. karta
							if (selectedCard[3] == false && currentGold - cardsExchange[3]->costGold >= 0) { //zaznaczam karte
								selectedCard[3] = true;
								currentGold -= cardsExchange[3]->costGold;
							}
							else if (selectedCard[3] == false) { //odznaczam karte
								selectedCard[3] = false;
								currentGold += cardsExchange[3]->costGold;
							}
							drawDealDealer(selectedCard, addStrength, addIntelligence, addVitality, currentGold, background);
						}
						else if (mouseX >= 850 && mouseY <= 1000 && cardsExchange.size() >= 5) { //5. karta
							if (selectedCard[4] == false && currentGold - cardsExchange[4]->costGold >= 0) { //zaznaczam karte
								selectedCard[4] = true;
								currentGold -= cardsExchange[4]->costGold;
							}
							else if (selectedCard[4] == false) { //odznaczam karte
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
							communication->sendDealNews(news);
							//mozna dopisac ewentualnie jakies wysiwtlenie ze udalo sie zakupic te rzeczy
							news = communication->receiveDealNews(); //pole news.accept
							mode = EXPLORE;
							NewsExplore nExp;
							nExp.gameMode = EXPLORE;
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
					if (mouseY >= 480 && mouseY <= 520) { //pasek przyscisku accept
						if (mouseX >= 800 && mouseX <= 950) { //przycisk akceptuj
							news.gameMode = DEAL;
							news.cardsId.clear();
							for (auto i = 0; i < 5; ++i) {
								news.cardsId.push_back(cardsExchange[i]->id);
							}
							communication->sendDealNews(news);
							//mozna dopisac ewentualnie jakies wysiwtlenie ze udalo sie zakupic te rzeczy
							news = communication->receiveDealNews(); //pole news.accept
							mode = EXPLORE;
							NewsExplore nExp;
							nExp.gameMode = EXPLORE;
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

void Game::drawExplore(Sprite &sidebar) {
	appWindow->clear(Color(150, 150, 150));
	currentLocation->drawBackground(appWindow);
	appWindow->draw(sidebar);
	Sprite sp;
	int x, y;
	bool paintedHero = false;
	bool paintedOpponent = false;
	for (auto it = currentLocation->objects.begin(); it != currentLocation->objects.end(); it++) {
		Object *obj = *it;
		/*if (obj->getVisibility() == false) //jezeli skrzynka lub mobek zostana odwiedzeni to s¹ oni usuwani z vecotr<Object*>objects w location
			continue;*/
		if (obj->getY() > mySquareY && !paintedHero) { //rysuj mojego bohatera w odpowiednim miejscu
			sp = myHero->getSprite();
			x = myX;
			y = myY - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
		}
		if (opponentLocationId == currentLocation->getId() && obj->getY() > opponentSquareY && !paintedOpponent) { //rysuj bohatera przeciwnka w opowiednim miejscu
			sp = opponentHero->getSprite();
			x = opponentSquareX * squareWidth;
			y = opponentSquareY * squareHeight - sp.getLocalBounds().height;
			sp.setPosition(Vector2f(x, y));
			appWindow->draw(sp);
		}
		sp = obj->getSprite();
		x = obj->getX() * squareWidth;
		y = obj->getY() * squareHeight - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	if (!paintedHero) { //rysuj mojego bohatera na samym dole mapy
		sp = myHero->getSprite();
		x = myX;
		y = myY - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	if (opponentLocationId == currentLocation->getId() && !paintedOpponent) { //rysuj bohatera przeciwnika na dole mapy
		sp = opponentHero->getSprite();
		x = opponentSquareX * squareWidth;
		y = opponentSquareY * squareHeight - sp.getLocalBounds().height;
		sp.setPosition(Vector2f(x, y));
		appWindow->draw(sp);
	}
	//rysowanie paska info
	Text tHp, tStat, tGold;
	/*tHp.setFont(font);
	tStat.setFont(font);
	tGold.setFont(font);
	tHp.setFillColor(Color(255, 51, 51));
	tStat.setFillColor(Color(255, 255, 255));
	tGold.setFillColor(Color(255, 255, 77));
	tHp.setCharacterSize(20);
	tStat.setCharacterSize(20);
	tGold.setCharacterSize(20);*/
	setText(tHp, 20, 255, 51, 51);
	setText(tStat, 20, 255, 255, 255);
	setText(tGold, 20, 255, 255, 77); //moze byc color: 212, 175, 55
	tHp.setPosition(mapSizeX + 5, 50);
	tGold.setPosition(mapSizeX + 5, 75);
	tStat.setPosition(mapSizeX + 5, 100);
	tHp.setString("HP: " + to_string(myHero->hp));
	tStat.setString(L"Si³a: " + to_string(myHero->strength) + L"\nInteligencja: " + to_string(myHero->intelligence) + L"\nWitalnoœæ: " + to_string(myHero->vitality));
	tGold.setString(L"Z³oto: " + to_string(myHero->gold));

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
	Sprite avatar1, avatar2; //avatar1 - opponent, avatar2 - myHero
	Texture tav1, tav2; //zaladowac obrazki z pliku
	avatar1.setTexture(tav1);
	avatar2.setTexture(tav2);
	avatar1.setPosition(25, 25);
	avatar2.setPosition(25, 375);
	appWindow->draw(avatar1);
	appWindow->draw(avatar2);
	//rysowanie odwrotow kart przeciwnika
	for (int i = 0; i < 5; ++i) {
		cardBack.setPosition(175 + i * 175, 25);
		appWindow->draw(cardBack);
	}
	//rysowanie moich kart
	for (int i = 0; i < 5; ++i) {
		myCard = myCardsOnHand[i]->sprite;
		myCard.setPosition(175 + i * 175, 325);
	}
	//narysowanie statysyk (HP)
	Text myHp, opHp;
	setText(myHp, 25, 212, 175, 55);
	setText(opHp, 25, 212, 175, 55);
	/*myHp.setFont(font);
	opHp.setFont(font);
	myHp.setCharacterSize(25);
	opHp.setCharacterSize(25);
	myHp.setFillColor(Color(212, 175, 55));
	opHp.setFillColor(Color(212, 175, 55));*/
	myHp.setString("HP: " + to_string(news.hp[0]));
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
	/*tDamage.setFont(font);
	tCostGold.setFont(font);
	tCostMana.setFont(font);
	tDamage.setFillColor(Color(255, 204, 204));
	tCostGold.setFillColor(Color(255, 255, 153));
	tCostMana.setFillColor(Color(153, 204, 255));
	tDamage.setCharacterSize(20);
	tCostGold.setCharacterSize(20);
	tCostMana.setCharacterSize(20);*/
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
	/*statName.setFont(font);
	statName.setFillColor(Color(255, 255, 255));
	statName.setCharacterSize(50);
	statPoints.setFont(font);
	statPoints.setFillColor(Color(255, 255, 255));
	statPoints.setCharacterSize(50);
	statCost.setFont(font);
	statCost.setFillColor(Color(255, 255, 255));
	statCost.setCharacterSize(25);
	statPlus.setFont(font);
	statPlus.setFillColor(Color(255, 255, 255));
	statPlus.setCharacterSize(50);*/
	setText(statName, 50, 255, 255, 255);
	setText(statPoints, 50, 255, 255, 255);
	setText(statCost, 25, 255, 255, 255);
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
	/*gold.setFont(font);
	gold.setFillColor(Color(255, 255, 255));
	gold.setCharacterSize(50);*/
	setText(gold, 50, 255, 255, 255);
	gold.setString(L"Masz z³ota: " + to_string(currentGold));
	gold.setPosition(450, 480);
	appWindow->draw(gold);
	/////// wyswietl przycisk accept
	RectangleShape accpetButton(sf::Vector2f(150, 40));
	accpetButton.setFillColor(Color(38, 13, 13));
	accpetButton.setPosition(800, 480);
	Text accept;
	/*accpet.setFont(font);
	accpet.setFillColor(Color(255, 255, 255));
	accpet.setCharacterSize(30);*/
	setText(accept, 30, 255, 255, 255);
	accept.setPosition(850, 485);
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
	RectangleShape accpetButton(sf::Vector2f(150, 40));
	accpetButton.setFillColor(Color(38, 13, 13));
	accpetButton.setPosition(800, 480);
	Text accpet;
	accpet.setFont(font);
	accpet.setFillColor(Color(255, 255, 255));
	accpet.setCharacterSize(30);
	accpet.setPosition(850, 485);
	appWindow->draw(accpetButton);
	appWindow->draw(accpet);
	
	appWindow->display();
}

void Game::setMySquare(const int & x, const int & y) {
	mySquareX = x;
	mySquareY = y;
}

void Game::setOponentSquare(const int & x, const int & y, const int & loc) {
	opponentSquareX = x;
	opponentSquareY = y;
	opponentLocationId = loc;
}

void Game::setAdjacent(int index, int val){
	adjacent[index] = val;
}

void Game::setText(sf::Text & text, const int & fontSize, const int & r, const int & g, const int & b) {
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(Color(r, g, b));
}


