#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

using namespace sf;

int main()
{
	int areaSizeX = 50;
	int areaSizeY = 25;
	enum GamePhase {INTRO, PRONTO, GAME} PHASE = INTRO;
	int licznik = 0;

	float x = 0;		//pozycja
	float y = 0;
	int xArea = 0;		//pozycja w polach
	int yArea = 0;
 	int response = 0;

	std::string s;
        Texture tempTexture;
	Sprite tempSprite;


	sf::RenderWindow window(sf::VideoMode(1114, 572), "Encounter");

	
	//GAME
	sf::Texture map;		//ładuje tekstury
	if (!map.loadFromFile("../img/mapka2.png"))
	{
		std::cout << "ups..." << std::endl;
	}
	sf::Texture intro;
	if (!intro.loadFromFile("../img/intro.png"))
	{
		std::cout << "ups..." << std::endl;
	}
	sf::Texture ziomek;
	if (!ziomek.loadFromFile("../img/ziomek.png"))
	{
		std::cout << "ups..." << std::endl;
	}
	
	sf::Sprite back;		//tworzy sprite'y z tekstur	
		back.setTexture(map);
	sf::Sprite authors;
		authors.setTexture(intro);
		authors.setScale(sf::Vector2f(0.2f, 0.2f));
		authors.setPosition(sf::Vector2f(0.f, 50.f));
	sf::Sprite hero;
		hero.setTexture(ziomek);
		hero.setScale(sf::Vector2f(0.1f, 0.1f));
	
	float heroHeight = hero.getLocalBounds().height;	//chcemy pamiętać lewy, dolny róg; domyślny jest lewy, górny
	hero.setOrigin(0.0f, heroHeight);
	
	x = hero.getPosition().x;	//ustaw x, y
	y = hero.getPosition().y;


	while (window.isOpen())		//główna pętla
    	{
			if(PHASE == GAME)
			{
					sf::Event event;
					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
						    window.close();
						
						// zakomentowane fragmenty mają pozwalać na kontrolowanie dostępu do kolejnych pól
						// możemy wymagać zgody (0 na std in) za każdym razem gdy postać przechodzi na nowe pole
						
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	//poruszanie się strzałkami
						{
							// right key is pressed: move our character
							hero.move(sf::Vector2f(2.f, 0.f));
							x = hero.getPosition().x;
							y = hero.getPosition().y;
							/*if(xArea != static_cast<int>(x/areaSizeX) || yArea != static_cast<int>(y/areaSizeY)){
								std::cout << static_cast<int>(x/areaSizeX) << " " << static_cast<int>(y/areaSizeY) << std::endl;
								std::cin >> response;
								if(response != 0){
									hero.move(sf::Vector2f(-2.f, 0.f));
								}
							}*/
							xArea = x/areaSizeX;
							yArea = y/areaSizeY;
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						{
							// left key is pressed: move our character
							hero.move(sf::Vector2f(-2.f, 0.f));
							x = hero.getPosition().x;
							y = hero.getPosition().y;
							/*if(xArea != static_cast<int>(x/areaSizeX) || yArea != static_cast<int>(y/areaSizeY)){
								std::cout << static_cast<int>(x/areaSizeX) << " " << static_cast<int>(y/areaSizeY) << std::endl;
								std::cin >> response;
								if(response != 0){
									hero.move(sf::Vector2f(2.f, 0.f));
								}
							}*/
							xArea = x/areaSizeX;
							yArea = y/areaSizeY;
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						{
							// up key is pressed: move our character
							hero.move(sf::Vector2f(0.f, -2.f));
							x = hero.getPosition().x;
							y = hero.getPosition().y;
							/*if(xArea != static_cast<int>(x/areaSizeX) || yArea != static_cast<int>(y/areaSizeY)){
								std::cout << static_cast<int>(x/areaSizeX) << " " << static_cast<int>(y/areaSizeY) << std::endl;
								std::cin >> response;
								if(response != 0){
									hero.move(sf::Vector2f(0.f, 2.f));
								}
							}*/
							xArea = x/areaSizeX;
							yArea = y/areaSizeY;
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						{
							// down key is pressed: move our character
							hero.move(sf::Vector2f(0.f, 2.f));
							x = hero.getPosition().x;
							y = hero.getPosition().y;
							/*if(xArea != static_cast<int>(x/areaSizeX) || yArea != static_cast<int>(y/areaSizeY)){
								std::cout << static_cast<int>(x/areaSizeX) << " " << static_cast<int>(y/areaSizeY) << std::endl;
								std::cin >> response;
								if(response != 0){
									hero.move(sf::Vector2f(0.f, -2.f));
								}
							}*/
							xArea = x/areaSizeX;
							yArea = y/areaSizeY;
						}//if
						
					}//while
			}//else

     		window.clear(Color(255, 255, 255, 0));

		if(PHASE == INTRO){
		   	window.draw(authors);
			++licznik;
			if(licznik > 1000)			//niech intro trwa 1000 klatek
			{
				PHASE = PRONTO;
				licznik = 0; 			//będzie potrzebne później
			}//if
		}//if
		if(PHASE == PRONTO)
		{
			if(licznik > 23)
				PHASE = GAME;
			s = std::to_string(24 - licznik);
			tempTexture.loadFromFile("../img/" + s + ".png");
			tempSprite.setTexture(tempTexture);
			tempSprite.setScale(Vector2f(0.35f, 0.35f));
			tempSprite.setPosition(sf::Vector2f(50.f, 100.f));
			window.draw(tempSprite); 
			sleep(milliseconds(100));			
			++licznik;
		}
		if(PHASE == GAME)
		{
			window.draw(back);
			window.draw(hero);
		}//if	
        window.display();
    }//while(is open)

    return 0;
}
