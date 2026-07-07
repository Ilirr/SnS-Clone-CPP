#include "Atlas.h"
#include "Texture.h"
#include <iostream>

Atlas::Atlas(const std::string& texturePath)
{

	m_Texture = std::make_unique<Texture>(texturePath);
}
Atlas::~Atlas() = default;
void Atlas::addSprite(const std::string& name, float x, float y, float width, float height)
{
	// Interpret parameters as pixel rectangle: (x,y) = top-left pixel, width/height in pixels
	glm::vec2 minPx = glm::vec2(x, y);
	glm::vec2 maxPx = glm::vec2(x + width, y + height);
	SubTexture2D subTex = SubTexture2D::CreateFromPixels(*m_Texture, minPx, maxPx);
	m_Sprites.emplace(name, std::move(subTex));

}
SubTexture2D Atlas::getSprite(const std::string& name) const
{
	auto sprite = m_Sprites.find(name);
	if (sprite != m_Sprites.end())
	{
		return sprite->second;
	}
	else {
		std::cerr << "Warning: Sprite '" << name << "' not found in atlas!\n";

		return SubTexture2D(*m_Texture, glm::vec4(0, 0, 1, 1));
	}

}
const Texture& Atlas::getTexture() const
{
	return *m_Texture;
}