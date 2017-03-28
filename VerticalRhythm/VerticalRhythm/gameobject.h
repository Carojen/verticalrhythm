#ifndef  GAMEOBJECT_H
#define GAMEOBJECT_H
#include <vector>
#include <SFML\Graphics\Shape.hpp>
#include <SFML\System\Vector2.hpp>

enum keyword;

namespace sf
{
	class sf::Time;
}

class GameObject
{
public:
	GameObject(keyword aType, sf::Vector2f position, sf::Vector2f size, float scale);
	~GameObject();

	void update(sf::Time& time);
	void draw();
	std::string toString();
	
	sf::Shape* GetShape();	
	sf::Vector2f GetPosition();
	void SetPosition(sf::Vector2f position);		
	sf::Vector2f GetSize();
	keyword GetType();
	std::vector<sf::Vector2f> GetPoints();
	
private:
	sf::Shape* mShape;
	void SetShape(keyword aType, sf::Vector2f size, float scale);
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	float mTime;
	std::vector<sf::Vector2f> mSurroundingPoints;
	sf::Vector2f mStartPoint;
	sf::Vector2f mEndPoint;
	keyword mType;
};
#endif 

