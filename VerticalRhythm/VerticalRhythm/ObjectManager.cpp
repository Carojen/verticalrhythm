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
	sf::Vector2f position = sf::Vector2f(-512, 0);
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
void ObjectManager::createTiles(std::vector<double> beats, sf::Vector2f offset)
{	
	sf::Vector2f position = sf::Vector2f(256, 0) + offset;
	
	for (auto beat : beats)
	{
		
		//std::cout << beat << " ";
		position.y += beat * 64 * 2 * 5;
		mTiles.push_back(new Tile(position));			
	}
	//std::cout << std::endl;	
}

std::vector<Tile*>& ObjectManager::GetTiles()
{
	return instance().mTiles;
}