#pragma once
#include "Game.h"
#include "News.h"
#include <SFML\Network.hpp>
#include <thread>
#include <mutex>
//#include <memory>
//extern std::mutex mutBlockCommunication;
//extern std::mutex protectData;
class Communication
{
	sf::IpAddress ip;
	sf::TcpSocket socket; //ten socket s³u¿y do obs³ugi ca³ej komuniakcji z serwerem
	//std::thread *th;
	std::unique_ptr <std::thread> thrUniquePtr;
	std::mutex mut;
public:
	Communication();
	~Communication();
	void startCommunication(Game &g);
	void receiveMap();

	void srData();
	void sendReceiveData(Game &game);
	//nowe podjescie w dol |
	void startExploreCommunicationInOnotherThread(Game &game); // //Najpierw odbiera NewsExplore, nastepnie wysy³a NewsExplore, ustawia dane pola game
	void exploreCommunication(Game &game);
	void exitCommunication();

	void sendExploreNews(const NewsExplore &news);
	void sendDealNews(const NewsDeal &news);
	void sendFightNews(const NewsFight &news);
	NewsExplore receiveExploreNews();
	NewsDeal receiveDealNews();
	NewsFight receiveFightNews();
	//void com();
};

