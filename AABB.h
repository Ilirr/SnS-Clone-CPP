#pragma once
#include <glm/glm.hpp>

struct AABB
{
	glm::vec2 min = { 0.0f, 0.0f };
	glm::vec2 max = { 0.0f, 0.0f };
};