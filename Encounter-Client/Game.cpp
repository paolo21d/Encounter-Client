#include "Game.h"
#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <SFML\Window.hpp>

using namespace sf;
using namespace std;


Game::Game(){
	appWindow = new RenderWindow(VideoMode(1114, 572, 32), "Encounter");
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
	tempSprite.setScale(Vector2f(0.25f, 0.25f));
	tempSprite.setPosition(sf::Vector2f(0, 0));
	appWindow->draw(tempSprite);
	appWindow->display();
	sleep(milliseconds(3000));

	for (int i = 0; i <= 23; ++i) {
		appWindow->clear(Color(255, 255, 255));
		cout << "Laduje " << i << endl;
		s = std::to_string(23 - i);
		tempTexture.loadFromFile("../img/" + s + ".png");
		tempSprite.setTexture(tempTexture);
		tempSprite.setScale(Vector2f(0.35f, 0.35f));
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
	Font font;
	font.loadFromFile("../img/Lato-Light.ttf");

	textName.setFont(font);
	textName.setCharacterSize(30);
	textName.setPosition(100, 300);

	hello.setCharacterSize(50);
	hello.setString(L"Witaj, wpisz swoje imi�  \nnast�pnie naci�nij enter aby rozpocz�� gr�");
	hello.setPosition(100, 100);
	hello.setFont(font);

	enterNameDraw(name, textName, hello);

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

			enterNameDraw(name, textName, hello);
		}
	}
}

void Game::enterNameDraw(string &name, Text &textName, Text &hello) {
	appWindow->clear(Color(150, 150, 150));
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
	Font font;
	font.loadFromFile("../img/Lato-Light.ttf");
	text.setFont(font);
	text.setCharacterSize(40);
	text.setPosition(100, 400);
	text.setFillColor(Color(255, 255, 255));
	text.setString(L"Nie mo�na si� po��czy� z serwerem!\nNaci�nij ESC");
	appWindow->clear(Color(50, 50, 50));
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

