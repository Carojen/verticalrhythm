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
	
	rhythm r;
	r.type = swing;
	r.length = 20;
	sf::Vector2f offset  = sf::Vector2f();			
	std::vector<double> beats = LevelGenerator::instance().GetBeats(r);

	


	std::vector<action> actions = LevelGenerator::instance().createActions(beats);
	std::vector<geometry> blocks = LevelGenerator::instance().GetGeometry(actions);
	ObjectManager::instance().addShapes(LevelGenerator::instance().GetShapes(blocks, offset));
		
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
			else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{							
				offset.x += 400;
				actions = LevelGenerator::instance().createActions(r);
				blocks = LevelGenerator::instance().GetGeometry(actions);
				ObjectManager::instance().addShapes(LevelGenerator::instance().GetShapes(blocks, offset));
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
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					view.move(-64, 0);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					view.move(64, 0);
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

		for (auto s : ObjectManager::instance().GetShapes())
		{
			window.draw(*s);
		}
		
		window.display();
	}
	

	return 0;
}
