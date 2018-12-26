#include "Game.h"
#include "Communication.h"
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;
int main()
{
	Game game;
	game.intro();
	Communication communication;
	try {
		communication.startCommunication();
	}
	catch (...) {
		cout << "Connection Error" << endl;
		return -2;
	}
	return 0;
}