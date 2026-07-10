#pragma once

#include "EntityID.h"
#include "Components.h"
#include <vector>
#include <cassert>

class EntityManager
{
public:
	EntityManager() = default;

	EntityID createEntity(const Entity& e)
	{
		if (!m_freeIndices.empty())
		{
			int idx = m_freeIndices.back();
			m_freeIndices.pop_back();
			m_entities[idx] = e;
			m_generations[idx] += 1;
			m_entities[idx].TransformComponent.prevPosition = m_entities[idx].TransformComponent.position;
			return EntityID{ idx, m_generations[idx] };
		}
		m_entities.push_back(e);
		m_generations.push_back(0);
		m_entities.back().TransformComponent.prevPosition = m_entities.back().TransformComponent.position;
		return EntityID{ static_cast<int>(m_entities.size() - 1), m_generations.back() };
	}

	void destroyEntity(const EntityID& id)
	{
		assert(valid(id) && "destroyEntity called with invalid handle");
		// Invalidate generation and make slot available
		m_generations[id.index] += 1;
		// Optional: reset entity to defaults
		m_entities[id.index] = Entity();
		m_freeIndices.push_back(id.index);
	}

	Entity& getEntity(const EntityID& id)
	{
		assert(valid(id) && "getEntity called with invalid handle");
		return m_entities[id.index];
	}
	const std::vector<Entity>& getAllEntities() const { return m_entities; }
	std::vector<Entity>& getAllEntities() { return m_entities; }
	size_t size() const { return m_entities.size(); }

	bool valid(const EntityID& id) const
	{
		if (id.index < 0) return false;
		if (static_cast<size_t>(id.index) >= m_entities.size()) return false;
		return m_generations[id.index] == id.generation;
	}

private:
	std::vector<Entity> m_entities;
	std::vector<uint32_t> m_generations;
	std::vector<int> m_freeIndices;
};
