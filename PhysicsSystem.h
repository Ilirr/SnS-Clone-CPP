#pragma once
#include "Components.h"
#include "EntityID.h"
#include "Scene.h"
#include <vector>

class PhysicsSystem : public SceneListener
{
public:
	PhysicsSystem();

	void updateEntity(TransformComponent& TransformComponent, ColliderComponent& ColliderComponent, RigidbodyComponent& body, double dt);
	bool checkAABB(const TransformComponent& tA, const ColliderComponent& cA, const TransformComponent& tB, const ColliderComponent& cB);

	const std::vector<Entity>& getEntities() const {
		static const std::vector<Entity> empty;
		return m_scene ? m_scene->getAllEntities() : empty;
	}

	void onAttach(Scene* scene) override;
	void onDetach() override;

	// SceneListener overrides
	void onEntityCreated(const EntityID& id) override;
	void onEntityDestroyed(const EntityID& id) override;

	void updateAll(double dt);

private:
	Scene* m_scene = nullptr;
};
