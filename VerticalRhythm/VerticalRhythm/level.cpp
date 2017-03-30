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
	
	float horizMove = 0;
	int nrOfHinders = 0;
	float averageX = 0;
	for (auto go : mGameObjects)
	{
		averageX += go->GetPosition().x;
		if (go->GetType() == move_left_hinder || go->GetType() == move_right_hinder || go->GetType() == moving_block_single)
		{
			nrOfHinders++;
		}
	}
	averageX /= mGameObjects.size();
	for (auto go : mGameObjects)
	{
		horizMove += abs(averageX - go->GetPosition().x);
	}
	horizMove /= mGameObjects.size();

	mLength = mActions.back().stoptime;
	mForgivenessRatio = (float)(mGameObjects.size() - nrOfHinders) / (float) mGameObjects.size();

	
	mLinearity = horizMove;

	UpdateOutline();
}
std::vector<GameObject*>& Level::GetGameObjects()
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
	std::cout << min_point;

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
	std::cout << " min: " << min_point << " max:" << max_point << std::endl;
	return mOutline;
}
Level::~Level()
{
}
