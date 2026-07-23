#include "CollisionWorld.h"
#include <algorithm>
#include <cmath>

void CollisionWorld::build(int w, int h, float tileSize)
{
    m_width = w;
    m_height = h;
    m_tileSize = tileSize;
    m_cells.assign(static_cast<size_t>(w) * h, TileCell{});
}

void CollisionWorld::clear()
{
    m_width = 0;
    m_height = 0;
    m_cells.clear();
    m_dynamics.clear();
}

void CollisionWorld::setSolid(int tx, int ty, bool solid)
{
    if (tx < 0 || ty < 0 || tx >= m_width || ty >= m_height)
        return;
    m_cells[static_cast<size_t>(ty) * m_width + tx].solid = solid;
}

bool CollisionWorld::isSolid(int tx, int ty) const
{
    if (tx < 0 || ty < 0 || tx >= m_width || ty >= m_height)
        return false;                       // out of bounds = empty air
    return m_cells[static_cast<size_t>(ty) * m_width + tx].solid;
}

AABB CollisionWorld::tileAABB(int tx, int ty) const
{
    const glm::vec2 lo(tx * m_tileSize, ty * m_tileSize);
    const glm::vec2 hi((tx + 1) * m_tileSize, (ty + 1) * m_tileSize);
    return { lo, hi };
}

bool CollisionWorld::overlapsTiles(const AABB& box) const
{
    const int x0 = static_cast<int>(std::floor(box.min.x / m_tileSize));
    const int x1 = static_cast<int>(std::floor(box.max.x / m_tileSize));
    const int y0 = static_cast<int>(std::floor(box.min.y / m_tileSize));
    const int y1 = static_cast<int>(std::floor(box.max.y / m_tileSize));

    for (int ty = y0; ty <= y1; ++ty)
        for (int tx = x0; tx <= x1; ++tx)
            if (isSolid(tx, ty))
                return true;

    return false;
}

void CollisionWorld::clearDynamics()
{
    m_dynamics.clear();  
}

void CollisionWorld::addDynamic(EntityID id, const AABB& box)
{
    m_dynamics.push_back({ id, box });
}

void CollisionWorld::queryDynamics(const AABB& box,
    std::vector<EntityID>& out) const
{
    out.clear();
    for (const auto& e : m_dynamics)
        if (e.aabb.overlaps(box))
            out.push_back(e.id);
}