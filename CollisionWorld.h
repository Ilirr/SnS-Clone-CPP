#pragma once
#include "AABB.h"
#include "EntityID.h"
#include <vector>

enum class TileType
{
    Empty = 0,
    Solid = 1,
    OneWay = 2
};

struct TileCell
{
    TileType type = TileType::Empty;
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

    void setTileType(int tx, int ty, TileType type);
    TileType getTileType(int tx, int ty) const;
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