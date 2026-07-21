#pragma once
#include "Components.h"
#include "EntityID.h"
#include "Scene.h"
#include "SpatialHashGrid.h"
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
	AABB getWorldAABB(EntityID entity) const;
	void rebuildStaticGrid();
	void rebuildDynamicGrid();
	void resolveCandidates(EntityID entity, const std::vector<EntityID>& candidates);

	Scene* m_scene = nullptr;
	SpatialHashGrid m_staticGrid{ 16.0f };
	SpatialHashGrid m_dynamicGrid{ 16.0f };
	std::vector<EntityID> m_staticCandidates;
	std::vector<EntityID> m_dynamicCandidates;
	bool m_staticGridDirty = true;
};
