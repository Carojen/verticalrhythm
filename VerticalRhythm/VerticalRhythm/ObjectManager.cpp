#include "ObjectManager.h"
#include <iostream>
#include "levelgenerator.h"

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
	sf::Vector2f position = sf::Vector2f(0, 512);
	for (int i = 0; i<20; i++)
	{
		mTiles.push_back(new Tile(position));
		position.x += 64;
	}

	std::cout << "Tiles created" << std::endl;
}

//Change this to take some other kind of input - not rhythms directly.
void ObjectManager::createTiles(std::vector<rhythm> rhythms)
{
	sf::Vector2f position = sf::Vector2f(0, 512);
	for (auto rhythm : rhythms)
	{
		for (auto beat : rhythm.beatPattern)
		{
			if (beat == 1)
			{
				mTiles.push_back(new Tile(position));
			}
			position.x += 64 * (3 - (int)rhythm.density);
		}
	}
	std::cout << "Tiles created from rhythm" << std::endl;
}

std::vector<Tile*>& ObjectManager::GetTiles()
{
	return instance().mTiles;
}