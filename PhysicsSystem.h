#pragma once
#include "Components.h"
#include "EntityID.h"
#include "Scene.h"
#include <vector>

class PhysicsSystem : public SceneListener
{
public:
	PhysicsSystem();

	void updateEntity(EntityID& entity, double dt);
	bool checkAABB(const EntityID& tA, const EntityID& tB);

	void onAttach(Scene* scene) override;
	void onDetach() override;

	// SceneListener overrides
	void onEntityCreated(const EntityID& id) override;
	void onEntityDestroyed(const EntityID& id) override;

	void updateAll(double dt);

private:
	Scene* m_scene = nullptr;
};
