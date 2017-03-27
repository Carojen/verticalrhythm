#include "level.h"
#include "gameobject.h"
#include "levelgenerator.h"

Level::Level()
{
}

Level::Level(std::vector<rhythm> rhythms, std::vector<action> actions, sf::Vector2f position)
{
	mRhythms = rhythms;
	mActions = actions;
	if (mActions.size() == 0)
	{
		float offset = 0;
		for (auto r : mRhythms)
		{
			for (auto a : LevelGenerator::instance().createActions(r, offset))
			{
				mActions.push_back(a);
			}
			offset = mActions.back().stoptime;
		}
	}
	mGameObjects = LevelGenerator::instance().GetLevelObjects(mActions, position);
}
std::vector<GameObject*> Level::GetGameObjects()
{
	return mGameObjects;
}
Level::~Level()
{
}
