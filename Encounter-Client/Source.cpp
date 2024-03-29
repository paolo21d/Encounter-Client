#include "Game.h"
#include "Communication.h"
#include <SFML/Window.hpp>
#include <iostream>
using namespace std;


int main()
{
	Communication communication;
	Game game(&communication);
	/*game.intro();
	try {
		game.enterName();
	}
	catch (...) {
		cout << "Zamykam" << endl;
		return 0;
	}
	try {
		communication.startCommunication(game);
	}
	catch (...) { //napisac komunikat ze nie mozna polaczyc a nie zamykac silowo
		cout << "Connection Error - cannot connect" << endl;
		game.cannnotConnect();
		return -2;
	}
	game.startGame();*/
	try {
		game.startGame();
	}
	catch (...) {
		cout << "Zamykam" << endl;
		communication.exitCommunication();
		return -1;
	}
	return 0;
}