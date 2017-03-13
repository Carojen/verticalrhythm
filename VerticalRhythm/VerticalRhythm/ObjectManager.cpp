#include "ObjectManager.h"
#include <iostream>
#include "levelgenerator.h"
#include <cmath>

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	while (mTiles.size() > 0)
	{
		delete mTiles[mTiles.size() - 1];
		mTiles.pop_back();
	}
}

ObjectManager& ObjectManager::instance()
{
	static ObjectManager om;
	return om;
}

void ObjectManager::createTiles()
{
	sf::Vector2f position = sf::Vector2f(0, 0);
	for (int i = 0; i<20; i++)
	{
		mTiles.push_back(new Tile(position));
		position.x += 512;
		mTiles.push_back(new Tile(position));
		position.x -= 512;
		position.y += 64;		
	}

	std::cout << "Tiles created" << std::endl;
}

//Change this to take some other kind of input - not rhythms directly.
void ObjectManager::createTiles(std::vector<rhythm> rhythms)
{
	sf::Vector2f position = sf::Vector2f(0, 512);
	for (auto rhythm : rhythms)
	{	
		int nrOfBeats = rhythm.type == RhythmType::regular ? 5 : 8;	
		switch (rhythm.density)
		{
		case low:
			nrOfBeats =  std::ceil((double)nrOfBeats / 2.0);
			break;
		case medium:
			break;
		case high:
			nrOfBeats *= 2;
		default:
			break;
		}
		nrOfBeats *= (double)rhythm.length/20.0;
		std::vector<double> beats;
		for (int i = 0; i < nrOfBeats; i++)
		{
			if (rhythm.type == RhythmType::regular)
			{
				beats.push_back(rhythm.length/nrOfBeats);
			}
			else
			{
				if (i % 2 == 0)
				{
					beats.push_back(rhythm.length / nrOfBeats * 4 / 5.0);
				}
				else
				{
					beats.push_back(rhythm.length / nrOfBeats * 1 / 5.0);
				}
			}
			
		}
		
		std::cout << "[";
		for (auto beat : beats)
		{
			std::cout << beat << ", ";
			position.y += beat * 64 * 4;
			mTiles.push_back(new Tile(position));			
		}
		std::cout << "]" << std::endl;
	}
	std::cout << "Tiles created from rhythm" << std::endl;
}

std::vector<Tile*>& ObjectManager::GetTiles()
{
	return instance().mTiles;
}