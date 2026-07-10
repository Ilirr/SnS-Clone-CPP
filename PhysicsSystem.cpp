#include "PhysicsSystem.h"
#include <algorithm>
#include <iostream>
#include <cassert>

PhysicsSystem::PhysicsSystem()
{

}
bool PhysicsSystem::checkAABB(const TransformComponent& tA, const ColliderComponent& cA, const TransformComponent& tB, const ColliderComponent& cB)
{
	// Compute world-space AABB for A
	glm::vec2 aMin = tA.position + cA.offset;
	glm::vec2 aMax = aMin + cA.size;

	// Compute world-space AABB for B
	glm::vec2 bMin = tB.position + cB.offset;
	glm::vec2 bMax = bMin + cB.size;

	// Check overlap on both axes (allow touching edges as collision)
	bool overlapX = (aMin.x <= bMax.x) && (aMax.x >= bMin.x);
	bool overlapY = (aMin.y <= bMax.y) && (aMax.y >= bMin.y);
	return overlapX && overlapY;
}
void PhysicsSystem::updateEntity(TransformComponent& TransformComponent, ColliderComponent& ColliderComponent, RigidbodyComponent& body, double dt)
{
	// Simple semi-implicit Euler integration
	const glm::vec2 gravity = glm::vec2(0.0f, 981.0f); // px/s^2
	body.velocity += gravity * body.gravityScale * static_cast<float>(dt);
	TransformComponent.position += body.velocity * static_cast<float>(dt);

	// Resolve against static bodies from the Scene (naive single-step positional correction)
	if (m_scene)
	{
		const auto& statics = m_scene->getStaticBodies();
		for (size_t i = 0; i < statics.size(); ++i)
		{
			const TransformComponent& sT = statics[i].TransformComponent;
			const ColliderComponent& sC = statics[i].ColliderComponent;

			if (!checkAABB(TransformComponent, ColliderComponent, sT, sC))
				continue;

		// Compute world-space mins/maxs
		glm::vec2 aMin = TransformComponent.position + ColliderComponent.offset;
		glm::vec2 aMax = aMin + ColliderComponent.size;
		glm::vec2 bMin = sT.position + sC.offset;
		glm::vec2 bMax = bMin + sC.size;

		// Centers
		glm::vec2 aCenter = (aMin + aMax) * 0.5f;
		glm::vec2 bCenter = (bMin + bMax) * 0.5f;
		glm::vec2 d = aCenter - bCenter; // from B to A

		float xOverlap = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
		float yOverlap = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);

		// Choose axis of least penetration
		glm::vec2 correction(0.0f);
		glm::vec2 normal(0.0f);
		if (xOverlap < yOverlap)
		{
			float sign = (d.x > 0.0f) ? 1.0f : -1.0f; // direction from B to A on X
			normal = glm::vec2(sign, 0.0f);
			correction = normal * xOverlap;
		}
		else
		{
			float sign = (d.y > 0.0f) ? 1.0f : -1.0f; // direction from B to A on Y
			normal = glm::vec2(0.0f, sign);
			correction = normal * yOverlap;
		}

		// Apply positional correction to move A out of penetration
		TransformComponent.position += correction;

		// Simple velocity correction: remove component of velocity into the ColliderComponent
		float vDot = glm::dot(body.velocity, normal);
		if (vDot < 0.0f)
		{
			body.velocity -= vDot * normal;
		}

		// ground check: if normal points up (negative Y in screen coords), consider grounded
		if (normal.y < 0.0f)
			body.isGrounded = true;
	}
	}
}

// Entity creation is handled by Scene; PhysicsSystem listens for entity lifecycle events.

void PhysicsSystem::updateAll(double dt)
{
	if (!m_scene) return;
	auto& TransformComponents = m_scene->getTransformComponents();
	auto& ColliderComponents = m_scene->getColliderComponents();
	auto& bodies = m_scene->getRigidBodies();

	size_t count = TransformComponents.size();
	for (size_t i = 0; i < count; ++i)
	{
		TransformComponent& t = TransformComponents[i];
		ColliderComponent& c = ColliderComponents[i];
		RigidbodyComponent& b = bodies[i];
		// reset grounded flag each frame
		b.isGrounded = false;
		// store previous position for render interpolation
		t.prevPosition = t.position;
		updateEntity(t, c, b, dt);
		// no previous-grounded bookkeeping or logging here
	}
}

// SceneListener callbacks
void PhysicsSystem::onEntityCreated(const EntityID& /*id*/)
{
	// no-op: we don't track previous grounded state anymore
}

void PhysicsSystem::onEntityDestroyed(const EntityID& /*id*/)
{
	// no-op
}

void PhysicsSystem::onAttach(Scene* scene)
{
	m_scene = scene;
}

void PhysicsSystem::onDetach()
{
	m_scene = nullptr;
}
