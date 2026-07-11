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

	void destroyEntity(EntityID& entity);

	void addListener(SceneListener* l);

	void removeListener(SceneListener* l);

	const std::vector<EntityID>& getActiveEntities() const { return m_activeEntities; };

	// non-const accessors
	TransformComponent* getTransform(EntityID entity) { return m_entityMgr.getTransform(entity); }
	RigidbodyComponent* getRigidbody(EntityID entity) { return m_entityMgr.getRigidbody(entity); }
	TagComponent* getTag(EntityID entity) { return m_entityMgr.getTag(entity); }
	SpriteComponent* getSprite(EntityID entity) { return m_entityMgr.getSprite(entity); }
	ColliderComponent* getCollider(EntityID entity) { return m_entityMgr.getCollider(entity); }

	// const accessors
	const TransformComponent* getTransform(EntityID entity) const { return m_entityMgr.getTransform(entity); }
	const RigidbodyComponent* getRigidbody(EntityID entity) const { return m_entityMgr.getRigidbody(entity); }
	const TagComponent* getTag(EntityID entity) const { return m_entityMgr.getTag(entity); }
	const SpriteComponent* getSprite(EntityID entity) const { return m_entityMgr.getSprite(entity); }
	const ColliderComponent* getCollider(EntityID entity) const { return m_entityMgr.getCollider(entity); }

	EntityManager& getEntityManager() { return m_entityMgr; }
	const EntityManager& getEntityManager() const { return m_entityMgr; }
	EntityID getPlayerEntity() const { return m_playerEntity; }
	void setPlayerEntity(const EntityID& entity) { m_playerEntity = entity; }

private: 

	std::vector<EntityID> m_activeEntities;

	EntityManager m_entityMgr;
	EntityID m_playerEntity;

	std::vector<SceneListener*> m_listeners;

};
