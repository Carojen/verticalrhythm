#ifndef  GAMEOBJECT_H
#define GAMEOBJECT_H
#include <vector>
#include <SFML\Graphics\Shape.hpp>
#include <SFML\System\Vector2.hpp>

enum keyword;

class GameObject
{
public:
	GameObject(keyword aType, sf::Vector2f position, sf::Vector2f size, float scale);
	~GameObject();

	void update();
	void draw();
	
	sf::Shape* GetShape();	
	sf::Vector2f GetPosition();
	void SetPosition(sf::Vector2f position);		
	sf::Vector2f GetSize();
	
private:
	sf::Shape* mShape;
	void SetShape(keyword aType, sf::Vector2f size, float scale);
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	std::vector<sf::Vector2f> mSurroundingPoints;
	sf::Vector2f mStartPoint;
	sf::Vector2f mEndPoint;
	keyword mType;
};
#endif 

