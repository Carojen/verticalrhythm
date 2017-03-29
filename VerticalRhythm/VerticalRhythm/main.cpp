#include <SFML/Graphics.hpp>
#include "ObjectManager.h"
#include <iostream>
#include <vector>
#include "levelgenerator.h"
#include "gameobject.h"
#include "level.h"


Level createLevel(sf::Vector2f offset = sf::Vector2f())
{
	rhythm r;
	std::vector<rhythm> rhythms;

	for (int i = 0; i < 10; i++)
	{
		r.density = (Density)(rand() % 3);
		r.length = ((rand() % 4) + 1) * 5;
		r.type = (RhythmType)(rand() % 2);
		rhythms.push_back(r);
		std::cout << "Rhythm " << i << ": " <<  MyEnums::ToString(r.density) << ", " << MyEnums::ToString(r.type) << ", " << r.length << std::endl;
	}

	Level level = Level(rhythms, std::vector<action>(), offset);
	ObjectManager::instance().addGameObjects(level.GetGameObjects());
	ObjectManager::instance().addShapes(level.GetOutline());


	for (auto go : level.GetGameObjects())
	{
		std::cout << go->toString() << std::endl;
	}

	std::cout << "Linearity: " << level.GetLinearity() << " Forgiveness ratio: " << level.GetForgivenessRatio() << " Length: " << level.GetLength() << std::endl;
	return level;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 728), "Rhythm");
	window.setFramerateLimit(60);
	std::cout << "Window created" << std::endl;

	sf::View view = window.getDefaultView();
	view.zoom(2.0f);
	window.setView(view);
			
	sf::Time time;
	sf::Clock clock;
	sf::Vector2f offset;
	bool drawShapes = true;

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
			else if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					offset.x += 1000;
					createLevel(offset);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{					
					ObjectManager::instance().ResetObjectManager();
					offset.x = 0;					
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
				{
					if (!isPaused)
					{
						std::cout << "paused game" << std::endl;
						drawShapes = false;
					}
					else
					{
						std::cout << "game unpaused" << std::endl;
						drawShapes = true;
					}
					isPaused = !isPaused;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
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
		for (auto go : ObjectManager::instance().GetGameObjects())
		{
			go->update(time);
		}

		time = clock.restart();

		if (!isPaused)
		{
		}
		
		window.clear();
		
		for (auto go : ObjectManager::instance().GetGameObjects())
		{
			window.draw(*(go->GetShape()));
		}
		for (auto shape : ObjectManager::instance().GetShapes())
		{
			if (drawShapes)
			{
				window.draw(*shape);
			}			
		}
		
		
		window.display();
	}
	

	return 0;
}
