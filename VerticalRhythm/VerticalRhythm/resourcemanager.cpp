#include "resourcemanager.h"
#include <iostream>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::instance()
{
	static ResourceManager rm;
	return rm;
}

bool ResourceManager::loadTexture(sf::String name)
{
	sf::Texture* tp = new sf::Texture();
	if (!tp->loadFromFile("Resources/" + name + ".png"))
	{
		std::cout << "Texture failed to load.";
		return false;
	}
	mTextures[name] = *tp;

	return true;
}
sf::Texture* ResourceManager::getTexture(sf::String name)
{
	if (mTextures.count(name) == 0)
	{
		loadTexture(name);
	}

	return &mTextures[name];
}
void ResourceManager::removeTexture(sf::String name)
{
	//mTextures.erase('name');
}