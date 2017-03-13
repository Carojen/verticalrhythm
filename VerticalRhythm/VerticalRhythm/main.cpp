#include <SFML/Graphics.hpp>
#include "tile.h"
#include "ObjectManager.h"
#include <iostream>
#include <vector>
#include "levelgenerator.h"
int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 728), "Rhythm");
	window.setFramerateLimit(60);
	std::cout << "Window created" << std::endl;

	sf::View view = window.getDefaultView();
	view.zoom(2.0f);
	window.setView(view);
	
	//ObjectManager::instance().createTiles();

	for (int d = 0; d < 3; d++)	
	{
		std::vector<rhythm> rhythms;
		for (int l = 1; l < 5; l++)
		{
			rhythm r;
			r.density = (Density)d;
			r.length = l*5;
			rhythms.push_back(r);
		}
		ObjectManager::instance().createTiles(rhythms, sf::Vector2f(64 * 2 * d, 0));
	}
	
	sf::Time time;
	sf::Clock clock;

	bool isPaused = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				if (!isPaused)
				{
					std::cout << "paused game" << std::endl;
				}
				else
				{
					std::cout << "game unpaused" << std::endl;
				}
				isPaused = !isPaused;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
				{
					view.zoom(0.75f);					
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
				{
					view.zoom(1.5f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					view.move(0, -64);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					view.move(0, 64);
				}
				window.setView(view);
			}
		}
		time = clock.restart();

		if (!isPaused)
		{
		}
		

		window.clear();

		for (auto t : ObjectManager::instance().GetTiles())
		{
			t->render(window);
		}
		
		window.display();
	}
	

	return 0;
}
