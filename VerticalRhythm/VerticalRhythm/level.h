#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "levelModel.h"
#include "SFML\Graphics\ConvexShape.hpp"


class GameObject;

namespace sf
{
	class Shape;
}


class Level
{
public:
	Level();
	~Level();
	Level(std::vector<rhythm> rhythms, std::vector<action> actions = std::vector<action>(), sf::Vector2f position = sf::Vector2f());
	std::vector<GameObject>& GetGameObjects();
	float GetLinearity();
	float GetLeniency();
	float GetLength();
	rhythm GetRhythm();
	std::vector<sf::ConvexShape>& GetOutline();
	std::vector<sf::ConvexShape>& UpdateOutline();

	float mLinearity[3] = {};
private:
	sf::Vector2f mPosition;
	std::vector<rhythm> mRhythms;
	std::vector<action> mActions;
	std::vector<GameObject> mGameObjects;
	
	float mLeniency;
	float mLength;
	std::vector<sf::ConvexShape> mOutline;
};

#endif
