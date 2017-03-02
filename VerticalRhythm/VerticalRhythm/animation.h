#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation();
	Animation(sf::Texture* texture, sf::IntRect startFrame, int length, int speed);
	~Animation();
	void update(sf::Time time, sf::Vector2f position);
	void render(sf::RenderWindow& window);
	sf::Sprite& getSprite();
private:
	sf::Sprite mSprite;
	sf::IntRect mRect;
	int mLength;
	int mSpeed;
	int mTime;
};

#endif