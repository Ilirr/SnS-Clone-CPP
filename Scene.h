#pragma once

#include "EntityManager.h"
#include "Components.h"
#include <vector>
#include <optional>

struct EntityID;

class SceneListener
{
public:
	virtual ~SceneListener() = default;
	virtual void onAttach(class Scene* scene) = 0;
	virtual void onDetach() = 0;
	virtual void onEntityCreated(const EntityID& id) = 0;
	virtual void onEntityDestroyed(const EntityID& id) = 0;
};

class Scene
{
public:
	Scene() = default;

	EntityID createEntity();
	void destroyEntity(EntityID entity);

	TransformComponent& getTransform(EntityID entity);

	const std::vector<EntityID>& getActiveEntities() const { return m_activeEntities; };

	EntityManager m_entityMgr;
	std::vector<SceneListener*> m_listeners;
	void addListener(SceneListener* l);
	void removeListener(SceneListener* l);


	void addComponent(EntityID entity, const TransformComponent& TransformComponent);

	void addComponent(EntityID entity, const SpriteComponent& sprite);

	void addComponent(EntityID entity, const RigidbodyComponent& rigidbody);

	void addComponent(EntityID entity, const TagComponent& tag);

private: 
	uint32_t m_nextEntityId = 0;
	std::vector<EntityID> m_activeEntities;

	std::unordered_map<EntityID, TransformComponent> m_TransformComponents;
	std::unordered_map<EntityID, SpriteComponent> m_sprites;
	std::unordered_map<EntityID, RigidbodyComponent> m_rigidBodies;

};
