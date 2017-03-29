#include "ObjectManager.h"
#include <iostream>
#include "levelgenerator.h"
#include <cmath>

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	ResetObjectManager();
}

void ObjectManager::ResetObjectManager()
{
	while (mGameObjects.size() > 0)
	{
		delete mGameObjects[mGameObjects.size() - 1];
		mGameObjects.pop_back();
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
