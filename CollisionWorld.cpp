#include "CollisionWorld.h"
#include <algorithm>
#include <cmath>

void CollisionWorld::build(int w, int h, float tileSize)
{
    m_width = w;
    m_height = h;
    m_tileSize = tileSize;
    m_cells.assign(static_cast<size_t>(w) * static_cast<size_t>(h), TileCell{});
    m_dynamics.clear();
}

void CollisionWorld::clear()
{
    m_width = 0;
    m_height = 0;
    m_tileSize = 16.0f;
    m_cells.clear();
    m_dynamics.clear();
}

void CollisionWorld::setSolid(int tx, int ty, bool solid)
{
    if (tx < 0 || tx >= m_width || ty < 0 || ty >= m_height) return;
    m_cells[static_cast<size_t>(ty) * m_width + tx].solid = solid;
}

bool CollisionWorld::isSolid(int tx, int ty) const
{
    if (tx < 0 || tx >= m_width || ty < 0 || ty >= m_height) return false;
    return m_cells[static_cast<size_t>(ty) * m_width + tx].solid;
}

AABB CollisionWorld::tileAABB(int tx, int ty) const
{
    AABB box;
    box.min = glm::vec2(static_cast<float>(tx) * m_tileSize,
        static_cast<float>(ty) * m_tileSize);
    box.max = box.min + glm::vec2(m_tileSize, m_tileSize);
    return box;
}

bool CollisionWorld::overlapsTiles(const AABB& box) const
{
    if (m_width <= 0 || m_height <= 0) return false;

    // Tiny epsilon so we don't sample the next tile when the AABB
    // sits exactly on a boundary.
    const float eps = 0.0001f;

    int x0 = static_cast<int>(std::floor(box.min.x / m_tileSize));
    int y0 = static_cast<int>(std::floor(box.min.y / m_tileSize));
    int x1 = static_cast<int>(std::floor((box.max.x - eps) / m_tileSize));
    int y1 = static_cast<int>(std::floor((box.max.y - eps) / m_tileSize));

    for (int ty = y0; ty <= y1; ++ty)
        for (int tx = x0; tx <= x1; ++tx)
            if (isSolid(tx, ty)) return true;

    return false;
}

void CollisionWorld::queryDynamics(const AABB& box, std::vector<EntityID>& out) const
{
    
    out.clear();
    out.reserve(m_dynamics.size());
    for (const DynamicEntry& e : m_dynamics) {
        if (box.overlaps(e.aabb)) out.push_back(e.id);
    }
}

void CollisionWorld::clearDynamics()
{
    // keep capacity to avoid reallocs each frame
    m_dynamics.clear();
}

void CollisionWorld::addDynamic(EntityID id, const AABB& box)
{
    m_dynamics.push_back({ id, box });
}