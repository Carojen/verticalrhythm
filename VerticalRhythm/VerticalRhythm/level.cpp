#include "level.h"
#include "gameobject.h"
#include "levelgenerator.h"
#include <iostream>
#include <SFML\Graphics\ConvexShape.hpp>

Level::Level()
{
}

Level::Level(std::vector<rhythm> rhythms, std::vector<action> actions, sf::Vector2f position)
{
	mRhythms = rhythms;
	mActions = actions;
	mPosition = position;
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
		
	float horizontalMovement = 0;
	float averageX = 0;
	float varianceX = 0;
	float averageDiff = 0;
	float prevX = 0;
	if (!mGameObjects.empty())
	{
		prevX = mGameObjects.at(0)->GetPosition().x;
	}
	int nrOfHinders = 0;	

	for (auto go : mGameObjects)
	{
		averageDiff += abs(go->GetPosition().x - prevX);
		prevX = go->GetPosition().x;

		averageX += go->GetPosition().x;

		if (go->GetType() == move_left_hinder || go->GetType() == move_right_hinder || go->GetType() == moving_block_single)
		{
			nrOfHinders++;
		}
	}
	averageX /= mGameObjects.size();
	averageDiff /= mGameObjects.size();
	for (auto go : mGameObjects)
	{
		varianceX = pow(go->GetPosition().x - averageX, 2);
		horizontalMovement += abs(go->GetPosition().x - averageX);
	}
	varianceX /= mGameObjects.size();
	horizontalMovement /= mGameObjects.size();

	//std::cout << "Var, hM, diff: " << sqrt(varianceX) << ", " << horizontalMovement << ", " << averageDiff << std::endl;

	mLength = mActions.back().stoptime;
	mLeniency = (float)(mGameObjects.size() - nrOfHinders) / (float) mGameObjects.size();	
	
	mLinearity[0] = horizontalMovement;
	mLinearity[1] = sqrt(varianceX);
	mLinearity[2] = averageDiff;

	UpdateOutline();
}
std::vector<GameObject*>& Level::GetGameObjects()
{
	return mGameObjects;
}
float Level::GetLinearity()
{
	return mLinearity[0];
}
float Level::GetLeniency()
{
	return mLeniency;
}
float Level::GetLength()
{	
	return mActions.back().stoptime;
}
rhythm Level::GetRhythm()
{
	return mRhythms.at(0);
}
std::vector<sf::Shape*>& Level::GetOutline()
{
	return mOutline;
}

std::vector<sf::Shape*>& Level::UpdateOutline()
{
	mOutline.clear();
	std::vector<sf::Vector2f> rightSide;
	std::vector<sf::Vector2f> leftSide;
	int scale = LevelGenerator::instance().avatar.scale.y;
	int passWidth = 4 * scale;
	float min_point = mPosition.x;
	float max_point = mPosition.x;	

	for (auto go : mGameObjects)
	{
		for (int i = 0; i < go->GetPoints().size(); i++)
		{
			if (i < go->GetPoints().size() / 2)
			{
				rightSide.push_back(go->GetPoints()[i] + go->GetPosition());
				if (go->GetPoints()[i].x + go->GetPosition().x > max_point)
				{
					max_point = go->GetPoints()[i].x + go->GetPosition().x;
				}
			}
			else
			{
				leftSide.push_back(go->GetPoints()[i] + go->GetPosition());
				if (go->GetPoints()[i].x + go->GetPosition().x < min_point)
				{
					min_point = go->GetPoints()[i].x + go->GetPosition().x;
				}
			}
		}
	}

	sf::Color backgroundColor = sf::Color(255, 128, 0);
	for (int i = 1; i < rightSide.size() - 1; i++)
	{
		sf::ConvexShape* boundariesRight = new sf::ConvexShape();
		boundariesRight->setPointCount(4);
		boundariesRight->setOutlineThickness(0);
		boundariesRight->setOutlineColor(sf::Color::Red);
		boundariesRight->setFillColor(backgroundColor);

		boundariesRight->setPoint(0, rightSide[i - 1]);
		boundariesRight->setPoint(1, rightSide[i]);
		boundariesRight->setPoint(2, sf::Vector2f(max_point + passWidth * 3, rightSide[i].y));
		boundariesRight->setPoint(3, sf::Vector2f(max_point + passWidth * 3, rightSide[i - 1].y));
		mOutline.push_back(boundariesRight);
	}

	for (int i = 1; i < leftSide.size() - 1; i++)
	{
		sf::ConvexShape* boundariesLeft = new sf::ConvexShape();
		boundariesLeft->setPointCount(4);
		boundariesLeft->setOutlineThickness(0);
		boundariesLeft->setOutlineColor(sf::Color::Red);
		boundariesLeft->setFillColor(backgroundColor);

		boundariesLeft->setPoint(0, leftSide[i - 1]);
		boundariesLeft->setPoint(1, leftSide[i]);
		boundariesLeft->setPoint(2, sf::Vector2f(min_point - passWidth * 3, leftSide[i].y));
		boundariesLeft->setPoint(3, sf::Vector2f(min_point - passWidth * 3, leftSide[i - 1].y));
		mOutline.push_back(boundariesLeft);
	}
	
	return mOutline;
}
Level::~Level()
{
}
