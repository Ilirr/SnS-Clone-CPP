#include "Scene.h"

Scene::Scene(){}

EntityID Scene::createEntity()
{
	EntityID id = m_entityMgr.createEntity();
	// notify listeners
	for (auto* l : m_listeners)
	{
		if (l)
		{
			l->onEntityCreated(id);
		}
	}

}
void Scene::addListener(SceneListener* l)
{
	if (l)
	{
		l->onAttach(this);
		m_listeners.push_back(l);
	}
}
void Scene::destroyEntity(const EntityID& id)
{
	if (!valid(id)) return;
	for (auto* l : m_listeners)
	{
		if (l)
		{
			l->onEntityDestroyed(id);
		}
	}
	m_entityMgr.destroyEntity(id);
}