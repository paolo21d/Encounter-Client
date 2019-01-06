#pragma once
#include "Game.h"
#include <SFML\Network.hpp>
#include <thread>
#include <mutex>
class Communication
{
	sf::IpAddress ip;
	sf::TcpSocket socket;
	//sf::Packet packet;
	std::thread *th;
	std::mutex mut;
public:
	Communication();
	~Communication();
	void startCommunication(Game &g);
	void receiveMap();
	void srData();
	void sendReceiveData(Game &game);
	//void com();
};

