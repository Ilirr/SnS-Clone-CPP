#pragma once
#include "Components.h"
#include "EntityID.h"
#include "Scene.h"
#include <vector>

class PhysicsSystem : public SceneListener
{
public:
	PhysicsSystem();

	void updateEntity(EntityID entity, double dt);
	bool checkAABB(EntityID tA, EntityID tB);

	void onAttach(Scene* scene) override;
	void onDetach() override;

	// SceneListener overrides
	void onEntityCreated(EntityID id) override;
	void onEntityDestroyed(EntityID id) override;

	void updateAll(double dt);

private:
	Scene* m_scene = nullptr;
};
