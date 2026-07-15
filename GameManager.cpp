#include "GameManager.h"
#include "LevelLoader.h"
#include "Input.h"
#include "Atlas.h"
#include <GLFW/glfw3.h>

GameManager::GameManager(){}
void GameManager::init(Atlas& atlas)
{
	LevelLoader loader;

	m_scene.addListener(&m_physicsMgr);

	// initialize weapon definitions (resolve sprite IDs using the atlas)
	m_weaponRegistry.init(atlas);

	loader.loadLevel("assets/levels/levels1.txt", m_scene, atlas, m_weaponRegistry, 64.0f);

}
void GameManager::handleInput(const InputManager& input)
{
	m_playerInputSystem.update(m_scene, input);
	if (input.wasPressed(GLFW_KEY_F3))
	{
		m_showSpatialGrid = !m_showSpatialGrid;
	}
}
void GameManager::render(Renderer2D& renderer, Atlas& atlas, double alpha)
{ 
	m_rendererMgr.renderScene(m_scene, renderer, atlas, alpha);
	if (m_showSpatialGrid)
	{
		m_physicsMgr.renderDebugOverlay(renderer);
	}
}

void GameManager::update(double dt)
{
	m_camera.setPrevPosition(m_camera.getPosition());
	m_actionMgr.update(m_scene, static_cast<float>(dt));
	m_physicsMgr.updateAll(dt);
	m_weaponMgr.update(m_scene, static_cast<float>(dt)); 

	const EntityID p = m_scene.getPlayerEntity();
	auto p_transform = m_scene.getTransform(p);

	if (m_scene.getEntityManager().isEntityAlive(p) && p_transform)
	{
		float camX = p_transform->position.x - 400.0f;
		float camY = p_transform->position.y - 300.0f;
		m_camera.setPosition(glm::vec2(camX, camY));
		
	}
}
	
