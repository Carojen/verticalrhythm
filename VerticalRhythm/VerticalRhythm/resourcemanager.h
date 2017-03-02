#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <SFML/Graphics.hpp>
#include <map>

class ResourceManager
{
public:
	~ResourceManager();
	static ResourceManager& instance();

	bool loadTexture(sf::String name);
	sf::Texture* getTexture(sf::String name);
	void removeTexture(sf::String name);

private:
	ResourceManager();
	std::map<sf::String, sf::Texture> mTextures;
};

#endif