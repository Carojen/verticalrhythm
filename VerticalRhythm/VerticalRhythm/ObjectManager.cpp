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
	while (mShapes.size() > 0)
	{
		delete mShapes[mShapes.size() - 1];
		mShapes.pop_back();
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
	sf::Vector2f position = sf::Vector2f(0, 0) + offset;
	
	for (auto beat : beats)
	{
		
		//std::cout << beat << " ";
		position.y += beat * 64 * 2 * 5;
		mTiles.push_back(new Tile(position));			
	}
	//std::cout << std::endl;	
}

void ObjectManager::createTiles(std::vector<action> actions, sf::Vector2f offset)
{
	sf::Vector2f position = sf::Vector2f(256, 0) + offset;
	TileType type = tile;
	for (int i = 0; i < actions.back().stoptime * 4; i++)
	{
		position.y += 64;
		mTiles.push_back(new Tile(position, type));
	}
	position = sf::Vector2f(256, 0) + offset;
	for (auto a : actions)
	{
		position.y = a.starttime * 64 * 4 + offset.y;
		if (a.word == move)
		{
			type = a.direction == left ? leftTile : rightTile;			
		}
		else
		{
			type = brakeTile;
		}
		//std::cout << a.starttime << " -> " << position.y << std::endl;
		mTiles.push_back(new Tile(position, type));
	}

	//std::cout << std::endl;	
}

std::vector<Tile*>& ObjectManager::GetTiles()
{
	return instance().mTiles;
}

void ObjectManager::createLevel()
{
	int count = 0;
	for (int d = 0; d < 3; d++)
	{
		for (int l = 1; l < 5; l++)
		{
			rhythm r;
			r.density = (Density)d;
			r.length = l * 5;
			r.type = RhythmType::swing;
			count++;
			//createTiles(LevelGenerator::instance().GetActions(LevelGenerator::instance().GetBeats(r)), sf::Vector2f(64 * 20 * count, 0));
			LevelGenerator::instance().GetGeometry(LevelGenerator::instance().createActions(LevelGenerator::instance().GetBeats(r)));
		}
	}
	for (int d = 0; d < 3; d++)
	{
		for (int l = 1; l < 5; l++)
		{
			rhythm r;
			r.density = (Density)d;
			r.length = l * 5;
			r.type = RhythmType::regular;
			count++;
			//createTiles(LevelGenerator::instance().GetActions(LevelGenerator::instance().GetBeats(r)), sf::Vector2f(64 * 20 * count, 0));
			LevelGenerator::instance().GetGeometry(LevelGenerator::instance().createActions(LevelGenerator::instance().GetBeats(r)));
		}
	}
}

void ObjectManager::addShapes(std::vector<sf::Shape*> shapes)
{
	for (auto shape : shapes)
	{
		mShapes.push_back(shape);
	}
}

std::vector<sf::Shape*>& ObjectManager::GetShapes()
{
	return instance().mShapes;
}

std::vector<GameObject*>& ObjectManager::GetGameObjects()
{
	return mGameObjects;
}

void ObjectManager::addGameObjects(std::vector<GameObject*> objects)
{
	for(auto o : objects)
	{
		mGameObjects.push_back(o);
	}
}
