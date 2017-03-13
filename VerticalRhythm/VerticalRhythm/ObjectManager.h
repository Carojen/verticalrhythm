#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>
#include "tile.h"

struct rhythm;

class ObjectManager
{
public:	
	static ObjectManager& instance();
	~ObjectManager();
	void createTiles();
	void createTiles(std::vector<rhythm> rhythms, sf::Vector2f offset = sf::Vector2f(0.0f,0.0f));
	static std::vector<Tile*>& GetTiles();
private:
	ObjectManager();
	std::vector<Tile*> mTiles;
};

#endif
