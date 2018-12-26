#include "Game.h"
#include "Communication.h"
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;
int main()
{
	Game game;
	//game.intro();
	try {
		game.enterName();
	}
	catch (...) {
		cout << "Zamykam" << endl;
		return 0;
	}
	Communication communication;
	try {
		communication.startCommunication();
	}
	catch (...) { //napisac komunikat ze nie mozna polaczyc a nie zamykac silowo
		cout << "Connection Error - cannot connect" << endl;
		game.cannnotConnect();
		return -2;
	}
	return 0;
}