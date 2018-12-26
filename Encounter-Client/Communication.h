#pragma once
#include <SFML\Network.hpp>
class Communication
{
	sf::IpAddress ip;
	sf::TcpSocket socket;
	sf::Packet packet;

public:
	Communication();
	~Communication();
	void startCommunication();
	void receiveMap();
	//void com();
};

