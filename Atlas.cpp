#include "Atlas.h"
#include "Texture.h"
#include <iostream>

Atlas::Atlas(const std::string& texturePath)
{
	m_Textures.push_back(std::make_unique<Texture>(texturePath));
}
Atlas::~Atlas() = default;
int Atlas::addSprite(const std::string& name, float x, float y, float width, float height)
{
	if (m_Textures.empty() || !m_Textures.front()->isValid())
	{
		std::cerr << "Cannot add sprite '" << name << "' because the atlas texture is invalid.\n";
		return -1;
	}

	const Texture& texture = *m_Textures.front();
	SubTexture2D subTex = SubTexture2D::CreateFromPixels(
		texture, { x, y }, { x + width, y + height });

	m_SpriteList.push_back(std::move(subTex));
	int id = static_cast<int>(m_SpriteList.size() - 1);
	m_SpriteNameToID[name] = id;
	return id;
}

int Atlas::addSpriteFromTexture(const std::string& name, const std::string& texturePath,
	float x, float y, float width, float height)
{
	auto texture = std::make_unique<Texture>(texturePath);
	if (!texture->isValid())
	{
		std::cerr << "Cannot add sprite '" << name << "' because the texture is invalid.\n";
		return -1;
	}

	Texture* texturePtr = texture.get();
	m_Textures.push_back(std::move(texture));
	SubTexture2D subTex = SubTexture2D::CreateFromPixels(*texturePtr, { x, y }, { x + width, y + height });

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
		return SubTexture2D(*m_Textures.front(), glm::vec4(0, 0, 1, 1));
	}
}
const Texture& Atlas::getTexture() const
{
	return *m_Textures.front();
}