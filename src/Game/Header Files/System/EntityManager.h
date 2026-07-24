#pragma once

#include "../Registry/EntityID.h"
#include "../Registry/Components.h"
#include <vector>
#include <cassert>
#include <unordered_map>

class EntityManager
{
public:
	EntityManager() = default;

	EntityID createEntity();

	void destroyEntity(EntityID id);

	bool isEntityAlive(EntityID entity) const;

	void addComponent(EntityID entity, const TransformComponent& transform);
	void addComponent(EntityID entity, const RigidbodyComponent& rigidbody);
	void addComponent(EntityID entity, const SpriteComponent& sprite);
	void addComponent(EntityID entity, const ColliderComponent& collider);
	void addComponent(EntityID entity, const TagComponent& tag);
	void addComponent(EntityID entity, const WeaponComponent& weapon);
	void addComponent(EntityID entity, const IntentComponent& intent);

	TransformComponent* getTransform(EntityID entity);
	RigidbodyComponent* getRigidbody(EntityID entity);
	SpriteComponent* getSprite(EntityID entity);
	ColliderComponent* getCollider(EntityID entity);
	TagComponent* getTag(EntityID entity);
	WeaponComponent* getWeapon(EntityID entity);
	IntentComponent* getIntent(EntityID entity);

	const TransformComponent* getTransform(EntityID entity) const;
	const RigidbodyComponent* getRigidbody(EntityID entity) const;
	const SpriteComponent* getSprite(EntityID entity) const;
	const ColliderComponent* getCollider(EntityID entity) const;
	const TagComponent* getTag(EntityID entity) const;
	const WeaponComponent* getWeapon(EntityID entity) const;
	const IntentComponent* getIntent(EntityID entity) const;

private:

	std::vector<uint32_t> m_generations;
	std::vector<int> m_freeIndices;

	std::unordered_map<EntityID, TransformComponent, EntityIDHash> m_transforms;
	std::unordered_map<EntityID, RigidbodyComponent, EntityIDHash> m_rigidbodies;
	std::unordered_map<EntityID, SpriteComponent, EntityIDHash> m_sprites;
	std::unordered_map<EntityID, ColliderComponent, EntityIDHash> m_colliders;
	std::unordered_map<EntityID, TagComponent, EntityIDHash> m_tags;
	std::unordered_map<EntityID, WeaponComponent, EntityIDHash> m_weapons;
	std::unordered_map<EntityID, IntentComponent, EntityIDHash> m_intents;
};
