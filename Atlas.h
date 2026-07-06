#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Texture.h"
#include "SubTexture2D.h"

class Atlas
{
public:

	Atlas(const std::string& texturePath);
	
	void addSprite(const std::string& name, float x, float y, float width, float height);

	SubTexture2D getSprite(const std::string& name) const;

	const Texture& getTexture() const;

private:
	std::unique_ptr<Texture> m_Texture;

	std::unordered_map<std::string, SubTexture2D> m_Sprites;

};