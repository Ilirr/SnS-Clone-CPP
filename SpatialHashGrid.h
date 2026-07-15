#pragma once
#include "EntityID.h"
#include <glm/glm.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct AABB
{
	glm::vec2 min = { 0.0f, 0.0f };
	glm::vec2 max = { 0.0f, 0.0f };
};

struct GridCell
{
	glm::ivec2 gridPosition = { 0, 0 };

	bool operator==(const GridCell& other) const
	{
		return gridPosition == other.gridPosition;
	}
};
struct GridCellHash
{
	std::size_t operator()(const GridCell& cell) const noexcept
	{
		const std::size_t xHash = std::hash<int>{}(cell.gridPosition.x);
		const std::size_t yHash = std::hash<int>{}(cell.gridPosition.y);
		return xHash ^ (yHash + 0x9e3779b9u + (xHash << 6) + (xHash >> 2));
	}
};

class SpatialHashGrid
{
public:
	explicit SpatialHashGrid(float cellSize);

	void clear();

	void insert(EntityID entity, const AABB& bounds);

	void update(EntityID entity, const AABB& bounds);

	void remove(EntityID entity);

	void query(const AABB& bounds, std::vector<EntityID>& results);
	float getCellSize() const { return m_cellSize; }
	std::vector<GridCell> getOccupiedCells() const;

private:
	using EntitySet = std::unordered_set<EntityID, EntityIDHash>;

	GridCell worldToCell(const glm::vec2& position) const;
	void getCellBounds(const AABB& bounds, GridCell& minCell, GridCell& maxCell) const;
	std::vector<GridCell> getCellsForAABB(const AABB& bounds) const;
	void ensureQueryStampCapacity(EntityID entity);
	void beginQuery();

	float m_cellSize;
	std::unordered_map<GridCell, EntitySet, GridCellHash> m_cells;
	std::unordered_map<EntityID, std::vector<GridCell>, EntityIDHash> m_entityCells;
	std::vector<uint32_t> m_lastSeenQueryByIndex;
	uint32_t m_queryStamp = 0;
};