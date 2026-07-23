#pragma once
#include <glm/vec2.hpp>

struct AABB
{
    glm::vec2 min{ 0.0f };
    glm::vec2 max{ 0.0f };

    bool overlaps(const AABB& o) const
    {
        return min.x < o.max.x && max.x > o.min.x
            && min.y < o.max.y && max.y > o.min.y;
    }
};