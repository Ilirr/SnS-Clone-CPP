#include "PhysicsSystem.h"
#include "Components.h"
#include "Renderer2D.h"
#include "Scene.h"
#include <cmath>
#include <iostream>
#include <chrono>
namespace
{
	constexpr float Gravity = 900.0f;
	constexpr float BoundaryEpsilon = 0.0001f;

	AABB computeAABB(const TransformComponent& transform, const ColliderComponent& collider)
	{
		AABB box;
		box.min = transform.position + collider.offset;
		box.max = box.min + collider.size;
		return box;
	}

	int tileIndex(float coordinate, float tileSize)
	{
		return static_cast<int>(std::floor(coordinate / tileSize));
	}
}

void PhysicsSystem::onAttach(Scene* scene, CollisionWorld* world)
{
	m_scene = scene;
	m_world = world;
	syncDynamics();
}

void PhysicsSystem::onDetach()
{
	if (m_world)
	{
		m_world->clearDynamics();
	}

	m_scene = nullptr;
	m_world = nullptr;
}

void PhysicsSystem::updateAll(double dt)
{
	if (!m_scene || !m_world || dt <= 0.0 || m_world->tileSize() <= 0.0f)
	{
		return;
	}

	const float deltaTime = static_cast<float>(dt);
	for (const EntityID entity : m_scene->getActiveEntities())
	{
		TransformComponent* transform = m_scene->getTransform(entity);
		RigidbodyComponent* rigidbody = m_scene->getRigidbody(entity);
		const ColliderComponent* collider = m_scene->getCollider(entity);

		if (!transform || !rigidbody || !collider || !collider->isSolid)
		{
			continue;
		}

		moveAndResolve(entity, deltaTime);
	}

	syncDynamics();
}

void PhysicsSystem::moveAndResolve(EntityID entity, float dt)
{
	TransformComponent* transform = m_scene->getTransform(entity);
	RigidbodyComponent* rigidbody = m_scene->getRigidbody(entity);
	const ColliderComponent* collider = m_scene->getCollider(entity);
	if (!transform || !rigidbody || !collider)
	{
		return;
	}

	const float tileSize = m_world->tileSize();
	transform->prevPosition = transform->position;
	rigidbody->velocity.y += Gravity * rigidbody->gravityScale * dt;

	transform->position.x += rigidbody->velocity.x * dt;
	AABB bounds = computeAABB(*transform, *collider);
	const int y0 = tileIndex(bounds.min.y, tileSize);
	const int y1 = tileIndex(bounds.max.y - BoundaryEpsilon, tileSize);

	if (rigidbody->velocity.x > 0.0f)
	{
		const int tx = tileIndex(bounds.max.x - BoundaryEpsilon, tileSize);
		for (int ty = y0; ty <= y1; ++ty)
		{
			if (m_world->isSolid(tx, ty))
			{
				transform->position.x = static_cast<float>(tx) * tileSize - collider->offset.x - collider->size.x;
				rigidbody->velocity.x = 0.0f;
				break;
			}
		}
	}
	else if (rigidbody->velocity.x < 0.0f)
	{
		const int tx = tileIndex(bounds.min.x, tileSize);
		for (int ty = y0; ty <= y1; ++ty)
		{
			if (m_world->isSolid(tx, ty))
			{
				transform->position.x = static_cast<float>(tx + 1) * tileSize - collider->offset.x;
				rigidbody->velocity.x = 0.0f;
				break;
			}
		}
	}

	transform->position.y += rigidbody->velocity.y * dt;
	rigidbody->isGrounded = false;
	bounds = computeAABB(*transform, *collider);
	const int x0 = tileIndex(bounds.min.x, tileSize);
	const int x1 = tileIndex(bounds.max.x - BoundaryEpsilon, tileSize);

	if (rigidbody->velocity.y > 0.0f)
	{
		const int ty = tileIndex(bounds.max.y - BoundaryEpsilon, tileSize);
		for (int tx = x0; tx <= x1; ++tx)
		{
			if (m_world->isSolid(tx, ty))
			{
				transform->position.y = static_cast<float>(ty) * tileSize - collider->offset.y - collider->size.y;
				rigidbody->velocity.y = 0.0f;
				rigidbody->isGrounded = true;
				break;
			}
		}
	}
	else if (rigidbody->velocity.y < 0.0f)
	{
		const int ty = tileIndex(bounds.min.y, tileSize);
		for (int tx = x0; tx <= x1; ++tx)
		{
			if (m_world->isSolid(tx, ty))
			{
				transform->position.y = static_cast<float>(ty + 1) * tileSize - collider->offset.y;
				rigidbody->velocity.y = 0.0f;
				break;
			}
		}
	}
}

void PhysicsSystem::syncDynamics()
{
	if (!m_scene || !m_world)
	{
		return;
	}

	m_world->clearDynamics();
	for (const EntityID entity : m_scene->getActiveEntities())
	{
		const TransformComponent* transform = m_scene->getTransform(entity);
		const ColliderComponent* collider = m_scene->getCollider(entity);
		if (transform && collider)
		{
			m_world->addDynamic(entity, computeAABB(*transform, *collider));
		}
	}
}

AABB PhysicsSystem::getAABB(EntityID entity) const
{
	if (!m_scene)
	{
		return {};
	}

	const TransformComponent* transform = m_scene->getTransform(entity);
	const ColliderComponent* collider = m_scene->getCollider(entity);
	return transform && collider ? computeAABB(*transform, *collider) : AABB{};
}

void PhysicsSystem::renderDebugOverlay(Renderer2D& renderer) const
{
	if (!m_scene )
	{
		return;
	}
	for (const EntityID entity : m_scene->getActiveEntities())
	{
		const TransformComponent* transform = m_scene->getTransform(entity);
		const ColliderComponent* collider = m_scene->getCollider(entity);
		if (!transform || !collider)
		{
			continue;
		}

		const AABB bounds = computeAABB(*transform, *collider);
		renderer.drawRectOutline(bounds.min, bounds.max - bounds.min, 1.0f, collider->isSolid ? glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) : glm::vec4(1.0f, 0.75f, 0.0f, 1.0f));
	}

	if (m_world)
	{

		const int width = m_world->width();
		const int height = m_world->height();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				if (m_world->isSolid(x, y))
				{
					AABB box = m_world->tileAABB(x, y);
					// Draw ground boxes in blue so you can tell them apart!
					renderer.drawRectOutline(box.min, box.max - box.min, 1.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				}
			}
		}
	}
}