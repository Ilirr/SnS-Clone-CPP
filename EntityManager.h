#pragma once

#include "EntityID.h"
#include "Components.h"
#include <vector>
#include <cassert>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager() = default;

	EntityID createEntity();

	void destroyEntity(const EntityID& id);

	void addComponent(const EntityID& entity, const TransformComponent& transform);
	void addComponent(const EntityID& entity, const SpriteComponent& sprite);
	void addComponent(const EntityID& entity, const RigidbodyComponent& rigidbody);
	void addComponent(const EntityID& entity, const TagComponent& tag);
	void addComponent(const EntityID& entity, const ColliderComponent& collider);

	// non-const accessors
	TransformComponent* getTransform(EntityID entity);
	RigidbodyComponent* getRigidbody(EntityID entity);
	SpriteComponent* getSprite(EntityID entity);
	TagComponent* getTag(EntityID entity);
	ColliderComponent* getCollider(EntityID entity);

	// const accessors
	const TransformComponent* getTransform(EntityID entity) const;
	const RigidbodyComponent* getRigidbody(EntityID entity) const;
	const SpriteComponent* getSprite(EntityID entity) const;
	const ColliderComponent* getCollider(EntityID entity) const;
	const TagComponent* getTag(EntityID entity) const;


private:

	std::vector<uint32_t> m_generations;
	std::vector<int> m_freeIndices;

	std::unordered_map<EntityID, TransformComponent, EntityIDHash> m_transforms;
	std::unordered_map<EntityID, RigidbodyComponent, EntityIDHash> m_rigidbodies;
	std::unordered_map<EntityID, SpriteComponent, EntityIDHash> m_sprites;
	std::unordered_map<EntityID, ColliderComponent, EntityIDHash> m_colliders;
	std::unordered_map<EntityID, TagComponent, EntityIDHash> m_tags;
};
