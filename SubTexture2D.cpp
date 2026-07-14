#include "SubTexture2D.h"
#include <cassert>

SubTexture2D::SubTexture2D(const Texture& texture, const glm::vec4& uv)
	: m_Texture(&texture), m_UV(uv)
{
	float u0 = uv.x; float v0 = uv.y; float u1 = uv.z; float v1 = uv.w;
	m_TexCoords[0] = { u0, v0 };
	m_TexCoords[1] = { u1, v0 };
	m_TexCoords[2] = { u1, v1 };
	m_TexCoords[3] = { u0, v1 };
}

SubTexture2D SubTexture2D::CreateFromPixels(const Texture& texture, const glm::vec2& minPx, const glm::vec2& maxPx)
{
	assert(texture.getWidth() > 0 && texture.getHeight() > 0);
	float w = static_cast<float>(texture.getWidth());
	float h = static_cast<float>(texture.getHeight());

	glm::vec4 uv;
	uv.x = minPx.x / w; // u0
	uv.y = minPx.y / h; // v0
	uv.z = maxPx.x / w; // u1
	uv.w = maxPx.y / h; // v1

	return SubTexture2D(texture, uv);
}

SubTexture2D SubTexture2D::CreateFromCoords(const Texture& texture, const glm::ivec2& coord, const glm::vec2& cellSize)
{
	glm::vec2 minPx = glm::vec2(coord.x * static_cast<int>(cellSize.x), coord.y * static_cast<int>(cellSize.y));
	glm::vec2 maxPx = minPx + cellSize;
	return CreateFromPixels(texture, minPx, maxPx);
}
