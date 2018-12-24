#include <SFML/Window.hpp>

int main()
{
	sf::Time x = sf::seconds(2);
    sf::Window window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed){
				sf::Window okno(sf::VideoMode(300, 300), "Na pewno?");
				sf::sleep(x);
                okno.close();
				window.close();
			}
        }
    }

    return 0;
}
