#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>
#include "tile.h"

struct rhythm;
struct action;
class GameObject;

class ObjectManager
{
public:	
	static ObjectManager& instance();
	~ObjectManager();
	void createTiles();
	void createTiles(std::vector<double> beats, sf::Vector2f offset = sf::Vector2f(0.0f,0.0f));
	void createTiles(std::vector<action> actions, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f));
	static std::vector<Tile*>& GetTiles();
	void createLevel();
	void addShapes(std::vector<sf::Shape*> shapes);
	static std::vector<sf::Shape*>& GetShapes();
	std::vector<GameObject*>& GetGameObjects();
	void addGameObjects(std::vector<GameObject*> objects);
private:
	ObjectManager();
	std::vector<Tile*> mTiles;
	std::vector<sf::Shape*> mShapes;
	std::vector<GameObject*> mGameObjects;
};

#endif
