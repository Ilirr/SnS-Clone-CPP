#pragma once
#include "Components.h"
#include <vector>
class PhysicsSystem
{
public:
	PhysicsSystem();
	// Integrate a single entity's rigidbody into its transform (no collisions)
	void updateEntity(Transform& transform, Collider& collider, RigidBody& body, double dt);
	bool checkAABB(const Transform& tA, const Collider& cA, const Transform& tB, const Collider& cB);

	// Static collider management (use StaticBody to bundle transform+collider)
	void addStaticBody(const StaticBody& body);
	void clearStaticBodies();

	// Dynamic entity storage (AoS)
	int addEntity(const Entity& e);
	const std::vector<Entity>& getEntities() const { return m_entities; }
	const std::vector<StaticBody>& getStaticBodies() const { return m_staticBodies; }

	Entity& getEntity(int id) { return m_entities[id]; }

	void updateAll(double dt);


private:
	std::vector<StaticBody> m_staticBodies;
	std::vector<Entity> m_entities;
	// previous grounded state per-entity for debug toggles
	std::vector<char> m_prevGrounded;

};
