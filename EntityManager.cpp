#include "EntityManager.h"
#include <utility>

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

void EntityManager::destroyEntity(const EntityID& id)
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
	++m_generations[id.index];
	m_freeIndices.push_back(id.index);
}

void EntityManager::addComponent(const EntityID& entity, const TransformComponent& component)
{
	m_transforms[entity] = component;
}
void EntityManager::addComponent(const EntityID& entity, const SpriteComponent& component)
{
	m_sprites[entity] = component;
}
void EntityManager::addComponent(const EntityID& entity, const RigidbodyComponent& component)
{
	m_rigidbodies[entity] = component;

}
void EntityManager::addComponent(const EntityID& entity, const TagComponent& component)
{
	m_tags[entity] = component;

}
void EntityManager::addComponent(const EntityID& entity, const ColliderComponent& component)
{
	m_colliders[entity] = component;
}
const TransformComponent* EntityManager::getTransform(EntityID entity) const
{
	auto it = m_transforms.find(entity);
	if (it != m_transforms.end())
	{
		return &it->second;
	}
	return nullptr;
}

TransformComponent* EntityManager::getTransform(EntityID entity)
{
	return const_cast<TransformComponent*>(std::as_const(*this).getTransform(entity));
}

RigidbodyComponent* EntityManager::getRigidbody(EntityID entity)
{
	return const_cast<RigidbodyComponent*>(std::as_const(*this).getRigidbody(entity));
}

SpriteComponent* EntityManager::getSprite(EntityID entity)
{
	return const_cast<SpriteComponent*>(std::as_const(*this).getSprite(entity));
}

TagComponent* EntityManager::getTag(EntityID entity)
{
	return const_cast<TagComponent*>(std::as_const(*this).getTag(entity));
}

ColliderComponent* EntityManager::getCollider(EntityID entity)
{
	return const_cast<ColliderComponent*>(std::as_const(*this).getCollider(entity));
}
const RigidbodyComponent* EntityManager::getRigidbody(EntityID entity) const
{
	auto it = m_rigidbodies.find(entity);
	if (it != m_rigidbodies.end())
	{
		return &it->second;
	}
	return nullptr;
}
const SpriteComponent* EntityManager::getSprite(EntityID entity) const
{
	auto it = m_sprites.find(entity);
	if (it != m_sprites.end())
	{
		return &it->second;
	}
	return nullptr;
}
const TagComponent* EntityManager::getTag(EntityID entity) const
{
	auto it = m_tags.find(entity);
	if (it != m_tags.end())
	{
		return &it->second;
	}
	return nullptr;
}
const ColliderComponent* EntityManager::getCollider(EntityID entity) const
{
	auto it = m_colliders.find(entity);
	if (it != m_colliders.end())
	{
		return &it->second;
	}
	return nullptr;
}