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

	// Construct from pixel coordinates (x0,y0,x1,y1)
	static SubTexture2D CreateFromPixels(const Texture& texture, const glm::vec2& minPx, const glm::vec2& maxPx);

	// Construct from grid cell coords (coord = column,row) given cell size in pixels
	static SubTexture2D CreateFromCoords(const Texture& texture, const glm::ivec2& coord, const glm::vec2& cellSize);

	// Return pointer to an array of 4 texcoords matching the vertex order used by Renderer2D
	// (top-left, top-right, bottom-right, bottom-left)
	const glm::vec2* getTexCoords() const { return m_TexCoords; }

	// Return the underlying texture
	const Texture& getTexture() const { return *m_Texture; }

	// Return UV rect (u0, v0, u1, v1)
	glm::vec4 getUV() const { return m_UV; }

private:
	const Texture* m_Texture = nullptr;
	glm::vec2 m_TexCoords[4];
	glm::vec4 m_UV;
};
