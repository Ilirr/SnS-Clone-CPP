#include "PhysicsSystem.h"
#include <algorithm>
#include <iostream>

PhysicsSystem::PhysicsSystem()
{

}

bool PhysicsSystem::checkAABB(const Transform& tA, const Collider& cA, const Transform& tB, const Collider& cB)
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

void PhysicsSystem::updateEntity(Transform& transform, Collider& collider, RigidBody& body, double dt)
{
	// Simple semi-implicit Euler integration
	const glm::vec2 gravity = glm::vec2(0.0f, 981.0f); // px/s^2
	body.velocity += gravity * body.gravityScale * static_cast<float>(dt);
	transform.position += body.velocity * static_cast<float>(dt);

	// Resolve against static bodies (naive single-step positional correction)
	for (size_t i = 0; i < m_staticBodies.size(); ++i)
	{
		const Transform& sT = m_staticBodies[i].transform;
		const Collider& sC = m_staticBodies[i].collider;

		if (!checkAABB(transform, collider, sT, sC))
			continue;

		// Compute world-space mins/maxs
		glm::vec2 aMin = transform.position + collider.offset;
		glm::vec2 aMax = aMin + collider.size;
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
		transform.position += correction;

		// Simple velocity correction: remove component of velocity into the collider
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

int PhysicsSystem::addEntity(const Entity& e)
{
	m_entities.push_back(e);
	m_prevGrounded.push_back(static_cast<char>(e.body.isGrounded));
	// initialize previous position to current so interpolation has a valid base
	m_entities.back().transform.prevPosition = m_entities.back().transform.position;
	return static_cast<int>(m_entities.size() - 1);
}

void PhysicsSystem::updateAll(double dt)
{
	for (size_t i = 0; i < m_entities.size(); ++i)
	{
		Entity& e = m_entities[i];
		// remember previous grounded state
		char prev = (i < m_prevGrounded.size()) ? m_prevGrounded[i] : 0;
		// reset grounded flag each frame
		e.body.isGrounded = false;
		// store previous position for render interpolation
		e.transform.prevPosition = e.transform.position;
		updateEntity(e.transform, e.collider, e.body, dt);

		// if grounded state changed, log for debugging
		char now = e.body.isGrounded ? 1 : 0;
		if (now != prev)
		{
			std::cerr << "Entity " << i << " grounded=" << (now ? "true" : "false")
				<< " pos=(" << e.transform.position.x << "," << e.transform.position.y << ")\n";
		}
		if (i < m_prevGrounded.size()) m_prevGrounded[i] = now;
		else m_prevGrounded.push_back(now);
	}
}

void PhysicsSystem::addStaticBody(const StaticBody& body)
{
	m_staticBodies.push_back(body);
}

void PhysicsSystem::clearStaticBodies()
{
	m_staticBodies.clear();
}
