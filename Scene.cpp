#include "Scene.h"
#include <algorithm>

EntityID Scene::createEntity()
{
	EntityID id = m_entityMgr.createEntity();

	for (auto* l : m_listeners)
	{
		if (l)
		{
			l->onEntityCreated(id);
		}
	}
	m_activeEntities.push_back(id);
	return id;
}
void Scene::addListener(SceneListener* l)
{
	if (l)
	{
		l->onAttach(this);
		m_listeners.push_back(l);
	}
}
void Scene::removeListener(SceneListener* l)
{
	if (!l) return;
	auto it = std::find(m_listeners.begin(), m_listeners.end(), l);
	if (it != m_listeners.end())
	{
		(*it)->onDetach();
		m_listeners.erase(it);
	}
}

void Scene::clear()
{
	// Copy active entities because destroyEntity mutates m_activeEntities
	auto entities = m_activeEntities;
	for (auto id : entities)
	{
		destroyEntity(id);
	}
	// Ensure player entity is reset
	m_playerEntity = {};
}
void Scene::destroyEntity(EntityID id)
{
	if (std::find(m_activeEntities.begin(), m_activeEntities.end(), id) == m_activeEntities.end())
	{
		return;
	}

	m_entityMgr.destroyEntity(id);
	for (auto* l : m_listeners)
	{
		if (l)
		{
			l->onEntityDestroyed(id);
		}
	}
	m_activeEntities.erase(std::remove(m_activeEntities.begin(), m_activeEntities.end(), id), m_activeEntities.end());
	if (m_playerEntity == id)
	{
		m_playerEntity = {};
	}

}