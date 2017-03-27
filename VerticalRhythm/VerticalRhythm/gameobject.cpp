#include "gameobject.h"
#include <SFML\Graphics\ConvexShape.hpp>
#include "levelgenerator.h"



GameObject::GameObject(keyword aType, sf::Vector2f position, sf::Vector2f size, float scale)
	:mType(aType), mPosition(position), mSize(size)
{
	SetShape(mType, size, scale);
	mShape->setPosition(position);
}

GameObject::~GameObject()
{
}

void GameObject::update()
{
}


void GameObject::draw()
{
}


sf::Shape * GameObject::GetShape()
{
	return mShape;
}

sf::Vector2f GameObject::GetPosition()
{
	return mPosition;
}

void GameObject::SetPosition(sf::Vector2f position)
{
	sf::Vector2f offset = position - mShape->getPosition();
	mStartPoint += offset;
	mEndPoint += offset;
	for (auto point : mSurroundingPoints)
	{
		point += offset;
	}
	mShape->setPosition(position);
}

sf::Vector2f GameObject::GetSize()
{
	return mSize;
}

void GameObject::SetShape(keyword aType, sf::Vector2f size, float scale)
{
	sf::ConvexShape* shape = new sf::ConvexShape();
	float platformThickness = scale * 0.25;
	float passWidth = scale * 4;	
	
	switch (aType)
	{
	case move_left_ledge:
		shape->setOutlineThickness(-2);
		shape->setOutlineColor(sf::Color::Green);
		shape->setFillColor(sf::Color::Green);
		shape->setPointCount(4);
		shape->setPoint(0, sf::Vector2f(0, 0));
		shape->setPoint(1, sf::Vector2f(-size.x, size.y));
		shape->setPoint(2, sf::Vector2f(-size.x + platformThickness, size.y + platformThickness));
		shape->setPoint(3, sf::Vector2f(platformThickness, platformThickness));

		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(0) + sf::Vector2f(platformThickness * 2, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(platformThickness * 2, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(passWidth, size.y < scale ? size.y / 2 : passWidth));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(passWidth, 0));
		break;
	case move_right_ledge:
		shape->setOutlineThickness(-2);
		shape->setOutlineColor(sf::Color::Green);
		shape->setFillColor(sf::Color::Green);
		shape->setPointCount(4);
		shape->setPoint(0, sf::Vector2f(0, 0));
		shape->setPoint(1, sf::Vector2f(size.x, size.y));
		shape->setPoint(2, sf::Vector2f(size.x - platformThickness, size.y + platformThickness));
		shape->setPoint(3, sf::Vector2f(-platformThickness, platformThickness));

		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(passWidth, -(size.y < scale ? size.y / 2 : passWidth)));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(passWidth, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(0) - sf::Vector2f(platformThickness * 2, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(platformThickness * 2, 0));		
		break;

	case move_left_hinder:
		shape->setOutlineThickness(-2);
		shape->setOutlineColor(sf::Color::Red);
		shape->setFillColor(sf::Color::Red);
		shape->setPointCount(4);
		shape->setPoint(0, sf::Vector2f(0, 0));
		shape->setPoint(1, sf::Vector2f(-size.x, size.y));
		shape->setPoint(2, sf::Vector2f(-size.x + scale, size.y + scale));
		shape->setPoint(3, sf::Vector2f(scale, scale));

		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(0) + sf::Vector2f(scale, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(scale, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(passWidth, (size.y < scale ? size.y / 2 : passWidth)));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(passWidth, 0));
		break;
	case move_right_hinder:
		shape->setOutlineThickness(-2);
		shape->setOutlineColor(sf::Color::Red);
		shape->setFillColor(sf::Color::Red);
		shape->setPointCount(4);
		shape->setPoint(0, sf::Vector2f(0, 0));
		shape->setPoint(1, sf::Vector2f(size.x, size.y));
		shape->setPoint(2, sf::Vector2f(size.x - scale, size.y + scale));
		shape->setPoint(3, sf::Vector2f(-scale, scale));

		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(passWidth, -(size.y < scale ? size.y / 2 : passWidth)));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(passWidth, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(0) - sf::Vector2f(scale, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) - sf::Vector2f(scale, 0));		
		break;
	case moving_block_single:
		shape->setOutlineThickness(-4);
		shape->setOutlineColor(sf::Color::Blue);
		shape->setFillColor(sf::Color::Transparent);
		shape->setPointCount(4);
		shape->setPoint(0, sf::Vector2f(0, 0));
		shape->setPoint(1, sf::Vector2f(scale, scale));
		shape->setPoint(2, sf::Vector2f(0, scale * 2));
		shape->setPoint(3, sf::Vector2f(-scale, scale));

		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(1) + sf::Vector2f(passWidth, 0));
		mSurroundingPoints.push_back(shape->getPosition() + shape->getPoint(3) - sf::Vector2f(passWidth, 0));
		break;
	case platform:
		break;
	default:
		break;
	}
	mShape = shape;
}
