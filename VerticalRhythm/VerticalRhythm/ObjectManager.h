#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>
#include "tile.h"

class ObjectManager
{
public:	
	static ObjectManager& instance();
	~ObjectManager();
	void createTiles();
	static std::vector<Tile*>& GetTiles();
private:
	ObjectManager();
	std::vector<Tile*> mTiles;
};

#endif
