#include "PhysicsSystem.h"
#include "Renderer2D.h"
#include <algorithm>

PhysicsSystem::PhysicsSystem()
{

}

AABB PhysicsSystem::getWorldAABB(EntityID entity) const
{
	const TransformComponent* transform = m_scene->getTransform(entity);
	const ColliderComponent* collider = m_scene->getCollider(entity);
	if (!transform || !collider)
	{
		return {};
	}

	const glm::vec2 min = transform->position + collider->offset;
	return { min, min + collider->size };
}

void PhysicsSystem::rebuildStaticGrid()
{
	m_staticGrid.clear();

	for (EntityID entity : m_scene->getActiveEntities())
	{
		const ColliderComponent* collider = m_scene->getCollider(entity);
		if (!collider || !collider->isSolid || collider->owner.isValid() ||
			m_scene->getRigidbody(entity) || !m_scene->getTransform(entity))
		{
			continue;
		}

		m_staticGrid.insert(entity, getWorldAABB(entity));
	}

	m_staticGridDirty = false;
}

void PhysicsSystem::rebuildDynamicGrid()
{
	m_dynamicGrid.clear();

	for (EntityID entity : m_scene->getActiveEntities())
	{
		const ColliderComponent* collider = m_scene->getCollider(entity);
		if (!collider || !m_scene->getTransform(entity))
		{
			continue;
		}

		if (!m_scene->getRigidbody(entity) && !collider->owner.isValid())
		{
			continue;
		}

		m_dynamicGrid.insert(entity, getWorldAABB(entity));
	}
}

void PhysicsSystem::resolveCandidates(
	EntityID entity,
	const std::vector<EntityID>& candidates)
{
	TransformComponent* transform = m_scene->getTransform(entity);
	ColliderComponent* collider = m_scene->getCollider(entity);
	RigidbodyComponent* body = m_scene->getRigidbody(entity);
	if (!transform || !collider || !body)
	{
		return;
	}

	for (EntityID other : candidates)
	{
		if (other == entity || !m_scene->getEntityManager().isEntityAlive(other))
		{
			continue;
		}

		const TransformComponent* otherTransform = m_scene->getTransform(other);
		const ColliderComponent* otherCollider = m_scene->getCollider(other);
		if (!otherTransform || !otherCollider || !otherCollider->isSolid)
		{
			continue;
		}

		if ((collider->owner.isValid() && collider->owner == other) ||
			(otherCollider->owner.isValid() && otherCollider->owner == entity) ||
			!checkAABB(entity, other))
		{
			continue;
		}

		const glm::vec2 aMin = transform->position + collider->offset;
		const glm::vec2 aMax = aMin + collider->size;
		const glm::vec2 bMin = otherTransform->position + otherCollider->offset;
		const glm::vec2 bMax = bMin + otherCollider->size;
		const float xOverlap = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
		const float yOverlap = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

		if (xOverlap < yOverlap)
		{
			const float direction = (aMin.x < bMin.x) ? -1.0f : 1.0f;
			transform->position.x += direction * xOverlap;
			body->velocity.x = 0.0f;
		}
		else
		{
			const float direction = (aMin.y < bMin.y) ? -1.0f : 1.0f;
			transform->position.y += direction * yOverlap;
			body->velocity.y = 0.0f;
			if (direction < 0.0f)
			{
				body->isGrounded = true;
			}
		}
	}
}

bool PhysicsSystem::checkAABB(EntityID entityA, EntityID entityB)
{
	const TransformComponent* tA = m_scene->getTransform(entityA);
	const ColliderComponent* cA = m_scene->getCollider(entityA);
	const TransformComponent* tB = m_scene->getTransform(entityB);
	const ColliderComponent* cB = m_scene->getCollider(entityB);

	if (!tA || !cA || !tB || !cB) return false;

	// Compute world-space AABB for A
	glm::vec2 aMin = tA->position + cA->offset;
	glm::vec2 aMax = aMin + cA->size;

	// Compute world-space AABB for B
	glm::vec2 bMin = tB->position + cB->offset;
	glm::vec2 bMax = bMin + cB->size;

	// Check overlap on both axes (allow touching edges as collision)
	bool overlapX = (aMin.x <= bMax.x) && (aMax.x >= bMin.x);
	bool overlapY = (aMin.y <= bMax.y) && (aMax.y >= bMin.y);
	return overlapX && overlapY;
}
void PhysicsSystem::updateEntity(EntityID entity, double dt)
{
	if (!m_scene)
	{
		return;
	}

	TransformComponent* transform = m_scene->getTransform(entity);
	ColliderComponent* collider = m_scene->getCollider(entity);
	RigidbodyComponent* body = m_scene->getRigidbody(entity);
	if (!transform || !collider || !body)
	{
		return;
	}

	const glm::vec2 gravity(0.0f, 981.0f);
	body->velocity += gravity * body->gravityScale * static_cast<float>(dt);
	transform->position += body->velocity * static_cast<float>(dt);

	m_staticGrid.query(getWorldAABB(entity), m_staticCandidates);
	resolveCandidates(entity, m_staticCandidates);

	m_dynamicGrid.query(getWorldAABB(entity), m_dynamicCandidates);
	resolveCandidates(entity, m_dynamicCandidates);

	m_dynamicGrid.update(entity, getWorldAABB(entity));
}
void PhysicsSystem::updateAll(double dt)
{
	if (!m_scene) return;
	if (m_staticGridDirty)
	{
		rebuildStaticGrid();
	}

	rebuildDynamicGrid();

	for (EntityID entity : m_scene->getActiveEntities())
	{
		RigidbodyComponent* body = m_scene->getRigidbody(entity);
		TransformComponent* transform = m_scene->getTransform(entity);
		if (!body || !transform || !m_scene->getCollider(entity))
		{
			continue;
		}

		body->isGrounded = false;
		transform->prevPosition = transform->position;
		updateEntity(entity, dt);
	}
}

void PhysicsSystem::renderDebugOverlay(Renderer2D& renderer) const
{
	const auto drawGrid = [&renderer](const SpatialHashGrid& grid, const glm::vec4& color)
	{
		const float cellSize = grid.getCellSize();
		for (const GridCell& cell : grid.getOccupiedCells())
		{
			const glm::vec2 position = glm::vec2(cell.gridPosition) * cellSize;
			renderer.drawRectOutline(position, { cellSize, cellSize }, 1.0f, color);
		}
	};

	drawGrid(m_staticGrid, { 0.1f, 0.4f, 1.0f, 0.8f });
	drawGrid(m_dynamicGrid, { 1.0f, 0.2f, 0.1f, 0.8f });
}
void PhysicsSystem::onEntityCreated(EntityID id)
{
	m_staticGridDirty = true;
}
void PhysicsSystem::onEntityDestroyed(EntityID id)
{
	m_staticGrid.remove(id);
	m_dynamicGrid.remove(id);
	m_staticGridDirty = true;
}
void PhysicsSystem::onAttach(Scene* scene)
{
	m_scene = scene;
	m_staticGridDirty = true;
}

void PhysicsSystem::onDetach()
{
	m_staticGrid.clear();
	m_dynamicGrid.clear();
	m_scene = nullptr;
}
