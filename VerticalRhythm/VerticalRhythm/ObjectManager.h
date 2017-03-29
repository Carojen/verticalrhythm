#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include <vector>

struct rhythm;
struct action;
class GameObject;

namespace sf
{
	class Shape;
}

class ObjectManager
{
public:	
	static ObjectManager& instance();
	~ObjectManager();
	void createLevel();
	void addShapes(std::vector<sf::Shape*> shapes);
	static std::vector<sf::Shape*>& GetShapes();
	std::vector<GameObject*>& GetGameObjects();
	void addGameObjects(std::vector<GameObject*> objects);
	void ResetObjectManager();
private:
	ObjectManager();
	std::vector<sf::Shape*> mShapes;
	std::vector<GameObject*> mGameObjects;
};

#endif
