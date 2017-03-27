#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Shape.hpp>
#include "levelModel.h"

class GameObject;
class LevelGenerator
{
public:
	static LevelGenerator& instance();
	LevelGenerator();
	~LevelGenerator();
	rhythm GetRhythm();
	rhythmgroup GetRhythmGroup();
	std::vector<double> GetBeats(rhythm r);
	std::vector<action> createActions(std::vector<double> beats, float offset = 0);
	std::vector<action> createActions(rhythm r, float offset = 0);
	std::vector<geometry> GetGeometry(std::vector<action> actions);
	std::vector<geometry> createGeometry(rhythm r);
	std::vector<GameObject*> GetLevelObjects(std::vector<action> actions, sf::Vector2f offset);
	std::vector<GameObject*> GetLevelObjects(std::vector<geometry> geometryElements, sf::Vector2f offset);
	std::vector<sf::Shape*> GetShapes(std::vector<geometry> geometryElements, sf::Vector2f offset = sf::Vector2f());
	player avatar;
};

#endif
