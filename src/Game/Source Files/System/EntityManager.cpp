#include "EntityManager.h"
#include <utility>

bool EntityManager::isEntityAlive(EntityID entity) const
{
	if (entity.index < 0 || static_cast<size_t>(entity.index) >= m_generations.size())
	{
		return false;
	}
	return m_generations[entity.index] == entity.generation;
}

EntityID EntityManager::createEntity()
{
	if (!m_freeIndices.empty())
	{
		const int index = m_freeIndices.back();
		m_freeIndices.pop_back();
		return { index, m_generations[index] };
	}

	const int index = static_cast<int>(m_generations.size());
	m_generations.push_back(0);
	return { index, 0 };
}

void EntityManager::destroyEntity(EntityID id)
{
	if (id.index < 0 || static_cast<size_t>(id.index) >= m_generations.size() || m_generations[id.index] != id.generation)
	{
		return;
	}
	m_transforms.erase(id);
	m_rigidbodies.erase(id);
	m_sprites.erase(id);
	m_colliders.erase(id);
	m_tags.erase(id);
	m_weapons.erase(id);
	m_intents.erase(id);
	++m_generations[id.index];
	m_freeIndices.push_back(id.index);
}
void EntityManager::addComponent(EntityID entity, const WeaponComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_weapons[entity] = component;
}
void EntityManager::addComponent(EntityID entity, const TransformComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_transforms[entity] = component;
}
void EntityManager::addComponent(EntityID entity, const SpriteComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_sprites[entity] = component;
}
void EntityManager::addComponent(EntityID entity, const RigidbodyComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_rigidbodies[entity] = component;

}
void EntityManager::addComponent(EntityID entity, const TagComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_tags[entity] = component;

}
void EntityManager::addComponent(EntityID entity, const ColliderComponent& component)
{
	if (!isEntityAlive(entity)) return;
	m_colliders[entity] = component;
}
void EntityManager::addComponent(EntityID entity, const IntentComponent& intent)
{
	if (!isEntityAlive(entity)) return;
	m_intents[entity] = intent;
}
WeaponComponent* EntityManager::getWeapon(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;
	return const_cast<WeaponComponent*>(std::as_const(*this).getWeapon(entity));
}
TransformComponent* EntityManager::getTransform(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;
	return const_cast<TransformComponent*>(std::as_const(*this).getTransform(entity));
}

RigidbodyComponent* EntityManager::getRigidbody(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;
	return const_cast<RigidbodyComponent*>(std::as_const(*this).getRigidbody(entity));
}

SpriteComponent* EntityManager::getSprite(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;
	return const_cast<SpriteComponent*>(std::as_const(*this).getSprite(entity));
}

TagComponent* EntityManager::getTag(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;

	return const_cast<TagComponent*>(std::as_const(*this).getTag(entity));
}

ColliderComponent* EntityManager::getCollider(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;

	return const_cast<ColliderComponent*>(std::as_const(*this).getCollider(entity));
}
IntentComponent* EntityManager::getIntent(EntityID entity)
{
	if (!isEntityAlive(entity)) return nullptr;
	return const_cast<IntentComponent*>(std::as_const(*this).getIntent(entity));
}
const WeaponComponent* EntityManager::getWeapon(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;
	auto it = m_weapons.find(entity);
	if (it != m_weapons.end())
	{
		return &it->second;
	}
	return nullptr;
}
const TransformComponent* EntityManager::getTransform(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;
	auto it = m_transforms.find(entity);
	if (it != m_transforms.end())
	{
		return &it->second;
	}
	return nullptr;
}
const RigidbodyComponent* EntityManager::getRigidbody(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;

	auto it = m_rigidbodies.find(entity);
	if (it != m_rigidbodies.end())
	{
		return &it->second;
	}
	return nullptr;
}
const SpriteComponent* EntityManager::getSprite(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;

	auto it = m_sprites.find(entity);
	if (it != m_sprites.end())
	{
		return &it->second;
	}
	return nullptr;
}
const TagComponent* EntityManager::getTag(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;

	auto it = m_tags.find(entity);
	if (it != m_tags.end())
	{
		return &it->second;
	}
	return nullptr;
}
const ColliderComponent* EntityManager::getCollider(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;

	auto it = m_colliders.find(entity);
	if (it != m_colliders.end())
	{
		return &it->second;
	}
	return nullptr;
}
const IntentComponent* EntityManager::getIntent(EntityID entity) const
{
	if (!isEntityAlive(entity)) return nullptr;
	auto it = m_intents.find(entity);
	if (it != m_intents.end())
	{
		return &it->second;
	}
	return nullptr;
}