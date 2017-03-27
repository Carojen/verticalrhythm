#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "levelModel.h"

class GameObject;

class Level
{
public:
	Level();
	~Level();
	Level(std::vector<rhythm> rhythms, std::vector<action> actions = std::vector<action>(), sf::Vector2f position = sf::Vector2f());
	std::vector<GameObject*> GetGameObjects();
	float GetLinearity();
	float GetForgivenessRatio();
	float GetLength();
private:
	sf::Vector2f mPosition;
	std::vector<rhythm> mRhythms;
	std::vector<action> mActions;
	std::vector<GameObject*> mGameObjects;
	float mLinearity;
	float mForgivenessRatio;
	float mLength;
};

#endif
