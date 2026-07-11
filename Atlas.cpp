#include "Atlas.h"
#include "Texture.h"
#include <iostream>

Atlas::Atlas(const std::string& texturePath)
{

	m_Texture = std::make_unique<Texture>(texturePath);
}
Atlas::~Atlas() = default;
int Atlas::addSprite(const std::string& name, float x, float y, float width, float height)
{
	if (!m_Texture || !m_Texture->isValid())
	{
		std::cerr << "Cannot add sprite '" << name << "' because the atlas texture is invalid.\n";
		return -1;
	}

	glm::vec2 minPx = glm::vec2(x, y);
	glm::vec2 maxPx = glm::vec2(x + width, y + height);

	SubTexture2D subTex = SubTexture2D::CreateFromPixels(*m_Texture, minPx, maxPx);

	m_SpriteList.push_back(std::move(subTex));
	int id = static_cast<int>(m_SpriteList.size() - 1);
	m_SpriteNameToID[name] = id;
	return id;
}
int Atlas::getSpriteId(const std::string& name) const
{
	auto it = m_SpriteNameToID.find(name);
	if (it != m_SpriteNameToID.end())
		return it->second;
	return -1;
}
SubTexture2D Atlas::getSpriteById(int id) const
{
	if (id >= 0 && id < static_cast<int>(m_SpriteList.size()))
	{
		return m_SpriteList[id];
	}
	else
	{
		std::cerr << "Warning: Sprite ID " << id << " not found in atlas!\n";
		return SubTexture2D(*m_Texture, glm::vec4(0, 0, 1, 1));
	}
}
const Texture& Atlas::getTexture() const
{
	return *m_Texture;
}