#include <SFML/Graphics.hpp>
#include "ObjectManager.h"
#include <iostream>
#include <vector>
#include "levelgenerator.h"
#include "gameobject.h"
#include "level.h"


Level* createLevel(sf::Vector2f offset = sf::Vector2f())
{
	rhythm r;
	std::vector<rhythm> rhythms;

	/* Följande stycke modifieras för att skapa önskade rytmer och antal att använda i genereringen av en bana*/
	for (int i = 0; i < 10; i++)
	{
		r.density = (Density)(rand() % 3);
		r.length = ((rand() % 4) + 1) * 5;
		r.type = (RhythmType)(rand() % 2);
		rhythms.push_back(r);
		std::cout << "Rhythm " << i << ": " <<  MyEnums::ToString(r.density) << ", " << MyEnums::ToString(r.type) << ", " << r.length << std::endl;
	}

	/* Skapandet av banan baserat på ovanstående lista av rytmer samt utskrift av banans egenskaper */
	Level* level = new Level(rhythms, std::vector<action>(), offset);
	ObjectManager::instance().addGameObjects(level->GetGameObjects());
	ObjectManager::instance().addShapes(level->GetOutline());
	for (auto go : level->GetGameObjects())
	{
		std::cout << go->toString() << std::endl;
	}
	
	std::cout << "Linearity: " << level->GetLinearity() << " Leniency: " << level->GetLeniency() << " Length: " << level->GetLength() << std::endl;
	return level;
}

Level* createLevel(rhythm r, sf::Vector2f offset = sf::Vector2f())
{
	std::vector<rhythm> rhythms;
	float levelLength = 0;

	while (levelLength < 60)
	{
		rhythms.push_back(r);
		levelLength += r.length;
	}

	return new Level(rhythms, std::vector<action>(), offset);
}

std::vector<Level*> createTestLevels(int levelID)
{
	std::vector<Level*> levels;
	rhythm r;
	r.type =  levelID < 12 ? regular : swing;
	
	switch (levelID % 12)
	{
		case 0:
		case 1:
		case 2:
			r.length = 5;			
			break;
		case 3:
		case 4:
		case 5:
			r.length = 10;
			break;
		case 6:
		case 7:
		case 8:
			r.length = 15;
			break;
		case 9:
		case 10:
		case 11:
			r.length = 20;
			break;
		default:
			break;
	}
	
	r.density = (Density) (levelID % 3);
	
	
	for (int i = 0; i < 100; i++)
	{
		levels.push_back(createLevel(r));
	}
	Level* linearityMin = levels.back();
	Level* linearityMax = levels.back();
	Level* leniencyMin = levels.back();
	Level* leniencyMax = levels.back();
	float leniencyAverage = 0;
	float linearityAverage = 0;

	for (auto l : levels)
	{
		leniencyAverage += l->GetLeniency();
		linearityAverage += l->GetLinearity();
		if (l->GetLeniency() >= leniencyMax->GetLeniency())
		{
			leniencyMax = l;
		}
		if (l->GetLeniency() < leniencyMin->GetLeniency())
		{
			leniencyMin = l;
		}
		if (l->GetLinearity() >= linearityMax->GetLinearity())
		{
			linearityMax = l;
		}
		if (l->GetLinearity() < linearityMin->GetLinearity())
		{
			linearityMin = l;
		}
	}
	linearityAverage /= 100;
	leniencyAverage /= 100;

	std::vector<Level*> result;
	sf::Vector2f offset;
		
	result.push_back(linearityMin);
	result.push_back(leniencyMin);
	if (linearityMin != linearityMax)
	{
		result.push_back(linearityMax);
	}	
	if (linearityMax != leniencyMin)
	{
		result.push_back(leniencyMax);
	}

	std::cout << "Rhythm " << levelID << ": " << MyEnums::ToString(r.density) << ", " << MyEnums::ToString(r.type) << ", " << r.length << std::endl;
	if (result.size() < 2)
	{
		std::cout << "Discarded due to low variety of generated levels." << std::endl;
	}
	else
	{
		std::cout << "Linearity: " << linearityMin->GetLinearity() << " - " << linearityMax->GetLinearity() << " (Avg: " << linearityAverage << "). Leniency: " << leniencyMin->GetLeniency() << " - " << leniencyMax->GetLeniency() << " (Avg: " << leniencyAverage << ")" << std::endl;

		for (auto level : result)
		{
			std::cout << "Linearity: " << level->GetLinearity() << " Leniency: " << level->GetLeniency() << " Length: " << level->GetLength() << std::endl;
			
			ObjectManager::instance().addGameObjects(level->GetGameObjects());
			ObjectManager::instance().addShapes(level->GetOutline());
		}
	}
	
	return result;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 728), "Rhythm");
	window.setFramerateLimit(60);
	std::cout << "Window created" << std::endl;

	sf::View view = window.getDefaultView();
	view.zoom(2.0f);
	window.setView(view);

	std::vector<Level*> levels;
			
	sf::Time time;
	sf::Clock clock;
	sf::Vector2f offset;
	bool drawShapes = false;
	bool drawObjects =false;
	bool drawLevel = true;
	int currentlevelIndex = 0;

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
				{
					levels.push_back(createLevel(offset));
					offset.x += 1000;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
				{
					currentlevelIndex++;
					if (currentlevelIndex >= levels.size())
					{
						currentlevelIndex = 0;
					}
					rhythm r = levels.at(currentlevelIndex)->GetRhythm();
					std::cout << "Rhythm " << currentlevelIndex << ": " << MyEnums::ToString(r.density) << ", " << MyEnums::ToString(r.type) << ", " << r.length << std::endl;
					std::cout << "Linearity: " << levels.at(currentlevelIndex)->GetLinearity() << " Leniency: " << levels.at(currentlevelIndex)->GetLeniency() << " Length: " << levels.at(currentlevelIndex)->GetLength() << std::endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
				{
					currentlevelIndex--;
					if (currentlevelIndex < 0)
					{
						currentlevelIndex = levels.size() - 1;
					}
					rhythm r = levels.at(currentlevelIndex)->GetRhythm();
					std::cout << "Rhythm " << currentlevelIndex << ": " << MyEnums::ToString(r.density) << ", " << MyEnums::ToString(r.type) << ", " << r.length << std::endl;
					std::cout << "Linearity: " << levels.at(currentlevelIndex)->GetLinearity() << " Leniency: " << levels.at(currentlevelIndex)->GetLeniency() << " Length: " << levels.at(currentlevelIndex)->GetLength() << std::endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
				{
					std::cout << "Starting creation of 24 * 100 levels" << std::endl;
					int creationTime = time.asSeconds();
					for (int i = 0; i < 24; i++)
					{
						for (auto l : createTestLevels(i))
						{
							levels.push_back(l);
						}						
					}
					creationTime = time.asSeconds() - creationTime;
					std::cout << "Finished." << std::endl;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
				{
					view.zoom(0.5f);					
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
				{
					view.zoom(2.0f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					view.move(0, -128);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					view.move(0, 128);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				{
					view.move(-128, 0);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					view.move(128, 0);
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
		
		if (drawObjects)
		{
			for (auto go : ObjectManager::instance().GetGameObjects())
			{
				window.draw(*(go->GetShape()));
			}
		}		
		if (drawShapes)
		{
			for (auto shape : ObjectManager::instance().GetShapes())
			{
				window.draw(*shape);
			}
		}
		if (drawLevel && levels.size() > 0)
		{
			for (auto go : levels.at(currentlevelIndex)->GetGameObjects())
			{
				window.draw(*(go->GetShape()));
			}
			for (auto shape : levels.at(currentlevelIndex)->GetOutline())
			{
				window.draw(*shape);
			}
		}

		
		window.display();
	}	

	return 0;
}
