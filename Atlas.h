#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "SubTexture2D.h"

class Texture;
class Atlas
{
public:
	Atlas(const std::string& texturePath);
	~Atlas();
	void addSprite(const std::string& name, float x, float y, float width, float height);

	SubTexture2D getSprite(const std::string& name) const;

	const Texture& getTexture() const;

private:
	std::unique_ptr<Texture> m_Texture;

	std::unordered_map<std::string, SubTexture2D> m_Sprites;

};