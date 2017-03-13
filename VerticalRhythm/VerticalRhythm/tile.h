#ifndef TILE_H
#define TILE_H
#include <SFML/Graphics.hpp>

class Tile
{
public:
	Tile(sf::Vector2f position);
	Tile(sf::Vector2i position);
	~Tile();
	void update();
	void render(sf::RenderWindow& window);
	sf::FloatRect getBounds();
private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
};

#endif