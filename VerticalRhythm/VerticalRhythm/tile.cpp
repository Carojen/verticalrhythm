#include "tile.h"
#include <iostream>
#include "levelgenerator.h"
//#define DRAW_BOXES
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

Tile::Tile(sf::Vector2f position, TileType tileType)
{
	if (!mTexture.loadFromFile("Resources/tile.png"))
	{
		std::cout << "Texture not loaded" << std::endl;
	}
	mSprite.setTexture(mTexture);
	mSprite.setPosition(position);

	type = tileType;
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
	sf::RectangleShape block;
	switch (type)
	{
	case TileType::tile:
		window.draw(mSprite);
		break;
	case TileType::leftTile:
		block.setSize(sf::Vector2f(getBounds().width*3 * 2, getBounds().height *2 ));
		block.setOutlineThickness(2);
		block.setFillColor(sf::Color::Blue);
		block.setOutlineColor(sf::Color::Blue);
		block.setPosition(getBounds().left + block.getLocalBounds().width/3, getBounds().top);
		window.draw(block);
		break;
	case TileType::rightTile:
		block.setSize(sf::Vector2f(getBounds().width *3 * 2, getBounds().height * 2));
		block.setOutlineThickness(2);
		block.setFillColor(sf::Color::Green);
		block.setOutlineColor(sf::Color::Green);
		block.setPosition(getBounds().left - block.getLocalBounds().width, getBounds().top);
		window.draw(block);
		break;
	case TileType::brakeTile:
		block.setSize(sf::Vector2f(getBounds().width *2, getBounds().height  *2));
		block.setOutlineThickness(2);
		block.setFillColor(sf::Color::Yellow);
		block.setOutlineColor(sf::Color::Yellow);
		block.setPosition(getBounds().left, getBounds().top);
		window.draw(block);
		break;
	default:
		break;
	}
}

sf::FloatRect Tile::getBounds()
{
	return mSprite.getGlobalBounds();
}