#include "animation.h"
#include <iostream>

Animation::Animation()
{

}
Animation::Animation(sf::Texture* texture, sf::IntRect startFrame, int length, int speed)
	:mRect(startFrame), mLength(length), mSpeed(speed)
{
	mSprite.setTexture(*texture);
	mSprite.setTextureRect(startFrame);
	mTime = 0;
	std::cout << "Animation created" << std::endl;
}

Animation::~Animation()
{

}

void Animation::update(sf::Time time, sf::Vector2f position)
{
	mSprite.setPosition(position);

	mTime += time.asMilliseconds();

	if (mTime >= 1000 / mSpeed)
	{
		mTime = 0;
		mRect.left += mRect.width;
		if (mRect.left >= mRect.width*mLength)
		{
			mRect.left = 0;
		}
		mSprite.setTextureRect(mRect);
	}
}

void Animation::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

sf::Sprite& Animation::getSprite()
{
	return mSprite;
}