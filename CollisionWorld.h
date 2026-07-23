#pragma once
#include "AABB.h"
#include "EntityID.h"
#include <vector>

struct TileCell
{
    bool solid = false;
};

struct DynamicEntry
{
    EntityID id;
    AABB aabb;
};
class CollisionWorld
{
public:

    void  build(int w, int h, float tileSize);
    void  clear();

    void  setSolid(int tx, int ty, bool solid);
    bool  isSolid(int tx, int ty) const;
    AABB  tileAABB(int tx, int ty) const;

    bool  overlapsTiles(const AABB& box) const;

    void  queryDynamics(const AABB& box, std::vector<EntityID>& out) const;
    void  clearDynamics();
    void  addDynamic(EntityID id, const AABB& box); 

    int   width()    const { return m_width; }
    int   height()   const { return m_height; }
    float tileSize() const { return m_tileSize; }

private:
    int m_width = 0;
    int m_height = 0;
    float m_tileSize = 16.0f;
    std::vector<TileCell> m_cells;
    std::vector<DynamicEntry> m_dynamics;
};