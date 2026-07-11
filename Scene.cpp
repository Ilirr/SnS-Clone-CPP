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
void Scene::destroyEntity(EntityID& id)
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