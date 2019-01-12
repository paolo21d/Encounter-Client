#pragma once
#include "Map.h"
#include "News.h"
#include "Location.h"
#include "Character.h"
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <mutex>

const unsigned mapSizeX = 1000, mapSizeY = 550;
const unsigned areaSizeX = 100, areaSizeY = 50;
//const unsigned squareHeight = mapSizeY / areaSizeY;
//const unsigned squareWidth = mapSizeX / areaSizeX;
const unsigned squareHeight = 50;
const unsigned squareWidth = 100;
const unsigned infoWidth = 50;

const unsigned costStrength = 10;
const unsigned costIntelligence = 15;
const unsigned costVitality = 10;


//std::mutex mutBlockCommunication;
class Game {
	std::mutex protectData;
	Communication *communication; //ten obiekt tez jest wywolywany w osobnym watku, pamietac o mutexach jak sie na nim dziala
	int endGame; //0-trwa gra, 1-ja przegtralem (umarlem w walce), 2-ja wygralem(pokonalem przeciwnika), 3-przeciwnik uciekl(rozlaczyl sie)

	sf::RenderWindow *appWindow;
	sf::Font font;
	Map worldMap;
	Location* currentLocation;
	//enum Mode { EXPLORE, FIGHT, DEAL };
	int adjacent[4]; //0-góra, 1-prawa, 2-dó³, 3-lewa 
					 //0-mozna wejsc, 1-pole nieinteraktywne(nie mozna wejsc), 2-pole interaktywne 
	int myX, myY; //wspolrzedne gracza pixelowe
	int mySquareX, mySquareY; //wspolrzedne pola na ktorym sie znaduje moj bohater
	int opponentSquareX, opponentSquareY, opponentLocationId;
	//int squareHeight, squareWidth; //wysokosc i szerokosc pól
	Character* myHero;
	Character* opponentHero;
	//pola do dealowania
	std::vector<Card *> cardsExchange; //otrzymana od serwera w trybie deale karty ktore posiada dealer lub chest
	std::vector<Card *> pickedCards; //to bedzie wysylane do serwera w trybie deal (delaer lub chest) jako wybrane do 'zakupu' karty
	int income;
	bool accpet;
	bool blockDealConnect; //taki mutex jakby
	//pola do walki
	std::vector<Card *>myCardsOnHand;


public:
	Mode mode;

	Game();
	Game(Communication *comptr);
	~Game();
	void intro();
	void enterName();
	void enterNameDraw(std::string &name, sf::Text &textName, sf::Text &hello, sf::Sprite &background);
	void setMap(Map& recMap);
	void cannnotConnect();

	int startGame();
	void explore();
	void fight();
	void deal();

	void drawExplore(sf::Sprite &sidebar, sf::Text &tHp, sf::Text &tStat, sf::Text &tGold);
	void drawFight(sf::Sprite &background);
	void drawFightHideOpponentCard(sf::Sprite &background, const NewsFight &news);
	void drawFightShowOpponentCard(sf::Sprite &background, const NewsFight &news);
	void drawFightEnd(sf::Sprite &background, const NewsFight &news);
	void drawDealDealer(const bool *selectedCards, const unsigned &addStrength, const unsigned &addIntelligence, const unsigned &addVitality, const unsigned &currentGold, sf::Sprite &background);
	void drawDealChest(const bool *selectedCards, const unsigned &currentGold, sf::Sprite &background);
	void drawEndGame(std::string info);
	void drawWaitingForOpponent();

	void setMySquare(const int &x, const int &y);
	void setOponentSquare(const int &x, const int &y, const int &loc);
	void setAdjacent(int index, int val);
	void setMode(Mode mode_);
	Mode getMode();
	int getMySquareX();
	int getMySquareY();

	inline void setText(sf::Text &text, const int &fontSize, const int &r, const int &g, const int &b);
	inline void setCardsOnHand(const NewsFight &news);
	friend class Communication;
};

