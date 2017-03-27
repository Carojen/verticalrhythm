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
	
	float horizMove = 0;
	int nrOfHinders = 0;
	for (auto go : mGameObjects)
	{
		horizMove += std::abs(go->GetPosition().x - mPosition.x);
		if (go->GetType() == move_left_hinder || go->GetType() == move_right_hinder || go->GetType() == moving_block_single)
		{
			nrOfHinders++;
		}
	}
	mLength = mActions.back().stoptime;
	mForgivenessRatio = (float)(mGameObjects.size() - nrOfHinders) / (float) mGameObjects.size();
	mLinearity = 1 - horizMove / (mGameObjects.at(1)->GetPosition().y - mGameObjects.back()->GetPosition().y);
}
std::vector<GameObject*> Level::GetGameObjects()
{
	return mGameObjects;
}
float Level::GetLinearity()
{
	return mLinearity;
}
float Level::GetForgivenessRatio()
{
	return mForgivenessRatio;
}
float Level::GetLength()
{	
	return mActions.back().stoptime;
}
Level::~Level()
{
}
