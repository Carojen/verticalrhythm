#include "tile.h"
#include <iostream>
#include "tile.h"
#define DRAW_BOXES
Tile::Tile(sf::Vector2f position)
{
	if (!mTexture.loadFromFile("Resources/tile.png"))
	{
		std::cout << "Texture not loaded" << std::endl;
	}
	mSprite.setTexture(mTexture);
	mSprite.setPosition(position);

}

Tile::Tile(sf::Vector2i position)
{
	Tile(sf::Vector2f(position.x, position.y));
}

Tile::~Tile()
{
}

void Tile::update()
{
}

void Tile::render(sf::RenderWindow& window)
{
#ifdef DRAW_BOXES
	sf::RectangleShape box;
	box.setSize(sf::Vector2f(getBounds().width, getBounds().height));
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineThickness(2);
	box.setOutlineColor(sf::Color::Red);
	box.setPosition(getBounds().left, getBounds().top);

	window.draw(box);
#endif
	window.draw(mSprite);
}

sf::FloatRect Tile::getBounds()
{
	return mSprite.getGlobalBounds();
}