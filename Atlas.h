#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "SubTexture2D.h"
#include <vector>
class Texture;
class Atlas
{
public:
	Atlas(const std::string& texturePath);
	~Atlas();


	int addSprite(const std::string& name, float x, float y, float width, float height);
	int addSpriteFromTexture(const std::string& name, const std::string& texturePath,
		float x, float y, float width, float height);
	int getSpriteId(const std::string& name) const;

	SubTexture2D getSpriteById(int id) const;

	const Texture& getTexture() const;

private:
	std::vector<std::unique_ptr<Texture>> m_Textures;

	std::vector<SubTexture2D> m_SpriteList; // stable list of sprites for ID-based access

	std::unordered_map<std::string, int> m_SpriteNameToID; // map from sprite name to index in m_SpriteList
};