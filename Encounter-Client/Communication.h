#pragma once
#include <SFML\Network.hpp>
#include <thread>
class Communication
{
	sf::IpAddress ip;
	sf::TcpSocket socket;
	//sf::Packet packet;
	std::thread *th;
public:
	Communication();
	~Communication();
	void startCommunication();
	void receiveMap();
	void srData();
	//void com();
};

