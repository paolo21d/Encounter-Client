#include <iostream>
#include <thread>
#include <string>
#include "News.h"
#include <SFML/Network.hpp>
//#include <Windows.h>

using namespace std;

int main() {
	sf::Packet packet;
	char toSend;
	size_t sentAmount;
	News news;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char buffer[200];
	size_t received;

	sf::Socket::Status status = socket.connect(ip, 2003);

	if (status != sf::Socket::Done){
		cout << "coś nie pykło" << endl;
		return 0;
	}
	//sf::sleep(sf::milliseconds(1000));	
	while(1){
		socket.receive(packet);
		packet >> news;
		cout << news;

		cin >> toSend;
		socket.send(&toSend, 2, sentAmount);
	}
	return 0;
}
