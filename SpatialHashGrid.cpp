#include "SpatialHashGrid.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

SpatialHashGrid::SpatialHashGrid(float cellSize)
	: m_cellSize(cellSize)
{
	if (cellSize <= 0.0f)
	{
		throw std::invalid_argument("Spatial hash grid cell size must be positive.");
	}
}

void SpatialHashGrid::clear()
{
	m_cells.clear();
	m_entityCells.clear();
	std::fill(m_lastSeenQueryByIndex.begin(), m_lastSeenQueryByIndex.end(), 0u);
	m_queryStamp = 0;
}

void SpatialHashGrid::insert(EntityID entity, const AABB& bounds)
{
	if (!entity.isValid())
	{
		return;
	}

	remove(entity);

	std::vector<GridCell> occupiedCells = getCellsForAABB(bounds);
	for (const GridCell& cell : occupiedCells)
	{
		m_cells[cell].insert(entity);
	}

	m_entityCells.emplace(entity, std::move(occupiedCells));
}

void SpatialHashGrid::update(EntityID entity, const AABB& bounds)
{
	insert(entity, bounds);
}

void SpatialHashGrid::remove(EntityID entity)
{
	auto entityCellsIt = m_entityCells.find(entity);
	if (entityCellsIt == m_entityCells.end())
	{
		return;
	}

	for (const GridCell& cell : entityCellsIt->second)
	{
		auto cellIt = m_cells.find(cell);
		if (cellIt == m_cells.end())
		{
			continue;
		}

		cellIt->second.erase(entity);
		if (cellIt->second.empty())
		{
			m_cells.erase(cellIt);
		}
	}

	m_entityCells.erase(entityCellsIt);
}

void SpatialHashGrid::query(const AABB& bounds, std::vector<EntityID>& results)
{
	beginQuery();
	results.clear();

	GridCell minCell;
	GridCell maxCell;
	getCellBounds(bounds, minCell, maxCell);

	for (int y = minCell.gridPosition.y; y <= maxCell.gridPosition.y; ++y)
	{
		for (int x = minCell.gridPosition.x; x <= maxCell.gridPosition.x; ++x)
		{
			const GridCell cell{ { x, y } };
			auto cellIt = m_cells.find(cell);
			if (cellIt == m_cells.end())
			{
				continue;
			}

			for (EntityID entity : cellIt->second)
			{
				if (!entity.isValid())
				{
					continue;
				}

				ensureQueryStampCapacity(entity);
				const std::size_t index = static_cast<std::size_t>(entity.index);

				if (m_lastSeenQueryByIndex[index] == m_queryStamp)
				{
					continue;
				}

				m_lastSeenQueryByIndex[index] = m_queryStamp;
				results.push_back(entity);
			}
		}
	}
}

std::vector<GridCell> SpatialHashGrid::getOccupiedCells() const
{
	std::vector<GridCell> cells;
	cells.reserve(m_cells.size());
	for (const auto& [cell, entities] : m_cells)
	{
		cells.push_back(cell);
	}
	return cells;
}

GridCell SpatialHashGrid::worldToCell(const glm::vec2& position) const
{
	return { {
		static_cast<int>(std::floor(position.x / m_cellSize)),
		static_cast<int>(std::floor(position.y / m_cellSize))
	} };
}

void SpatialHashGrid::getCellBounds(
	const AABB& bounds,
	GridCell& minCell,
	GridCell& maxCell) const
{
	const float maxX = bounds.max.x > bounds.min.x
		? std::nextafter(bounds.max.x, -std::numeric_limits<float>::infinity())
		: bounds.min.x;
	const float maxY = bounds.max.y > bounds.min.y
		? std::nextafter(bounds.max.y, -std::numeric_limits<float>::infinity())
		: bounds.min.y;

	minCell = worldToCell(bounds.min);
	maxCell = worldToCell({ maxX, maxY });
}

std::vector<GridCell> SpatialHashGrid::getCellsForAABB(const AABB& bounds) const
{
	GridCell minCell;
	GridCell maxCell;
	getCellBounds(bounds, minCell, maxCell);

	const int width = maxCell.gridPosition.x - minCell.gridPosition.x + 1;
	const int height = maxCell.gridPosition.y - minCell.gridPosition.y + 1;
	std::vector<GridCell> cells;

	if (width <= 0 || height <= 0)
	{
		return cells;
	}

	cells.reserve(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));

	for (int y = minCell.gridPosition.y; y <= maxCell.gridPosition.y; ++y)
	{
		for (int x = minCell.gridPosition.x; x <= maxCell.gridPosition.x; ++x)
		{
			cells.push_back({ { x, y } });
		}
	}

	return cells;
}

void SpatialHashGrid::ensureQueryStampCapacity(EntityID entity)
{
	const std::size_t requiredSize = static_cast<std::size_t>(entity.index) + 1;
	if (m_lastSeenQueryByIndex.size() < requiredSize)
	{
		m_lastSeenQueryByIndex.resize(requiredSize, 0u);
	}
}

void SpatialHashGrid::beginQuery()
{
	++m_queryStamp;
	if (m_queryStamp == 0u)
	{
		std::fill(m_lastSeenQueryByIndex.begin(), m_lastSeenQueryByIndex.end(), 0u);
		m_queryStamp = 1u;
	}
}
