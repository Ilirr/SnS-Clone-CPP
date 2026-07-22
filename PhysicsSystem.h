#pragma once
#include "Components.h"
#include "EntityID.h"
#include "Scene.h"
#include "AABB.h"
#include <vector>

class PhysicsSystem : public SceneListener
{
public:
	PhysicsSystem();

	void updateEntity(EntityID entity, double dt);
	bool checkAABB(EntityID tA, EntityID tB);

	void onAttach(Scene* scene) override;
	void onDetach() override;

	void onEntityCreated(EntityID id) override;
	void onEntityDestroyed(EntityID id) override;

	void updateAll(double dt);
	void renderDebugOverlay(class Renderer2D& renderer) const;

private:
	AABB getAABB(EntityID entity) const;
	void resolveCandidates(EntityID entity, const std::vector<EntityID>& candidates);

	Scene* m_scene = nullptr;

	std::vector<EntityID> m_staticCandidates;
	std::vector<EntityID> m_dynamicCandidates;
};
