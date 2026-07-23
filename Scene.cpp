#include "Scene.h"
#include <algorithm>

EntityID Scene::createEntity()
{
	EntityID id = m_entityMgr.createEntity();

	m_activeEntities.push_back(id);
	return id;
}

void Scene::clear()
{
	auto entities = m_activeEntities;
	for (auto id : entities)
	{
		destroyEntity(id);
	}
	m_playerEntity = {};
}
void Scene::destroyEntity(EntityID id)
{
	if (std::find(m_activeEntities.begin(), m_activeEntities.end(), id) == m_activeEntities.end())
	{
		return;
	}

	m_entityMgr.destroyEntity(id);

	m_activeEntities.erase(std::remove(m_activeEntities.begin(), m_activeEntities.end(), id), m_activeEntities.end());
	if (m_playerEntity == id)
	{
		m_playerEntity = {};
		//Log player entity erased later here
	}

}