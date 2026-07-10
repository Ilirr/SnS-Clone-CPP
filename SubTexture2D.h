#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

// Small utility representing a sub-region (UV rectangle) of a Texture.
// Coordinates may be constructed from pixel coordinates and are converted
// to normalized UVs using the texture's width/height.
class SubTexture2D
{
public:
	SubTexture2D(const Texture& texture, const glm::vec4& uv);

	static SubTexture2D CreateFromPixels(const Texture& texture, const glm::vec2& minPx, const glm::vec2& maxPx);

	static SubTexture2D CreateFromCoords(const Texture& texture, const glm::ivec2& coord, const glm::vec2& cellSize);

	const glm::vec2* getTexCoords() const { return m_TexCoords; }

	const Texture& getTexture() const { return *m_Texture; }
	glm::vec4 getUV() const { return m_UV; }

private:
	const Texture* m_Texture = nullptr;
	glm::vec2 m_TexCoords[4];
	glm::vec4 m_UV;
};
