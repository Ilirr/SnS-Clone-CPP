#include "GameManager.h"
#include "LevelLoader.h"
#include "Input.h"
#include "Atlas.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <algorithm>

GameManager::GameManager(){}
void GameManager::init(Atlas& atlas)
{
	LevelLoader loader;
	CollisionWorld world;
	m_weaponRegistry.init(atlas);

	const glm::vec2 levelSize = loader.loadLevel("assets/levels/final.ldtk", m_scene, atlas, m_weaponRegistry, world, 16.0f);
	const glm::vec2 validLevelSize = (levelSize.x > 0.0f && levelSize.y > 0.0f) ? levelSize : glm::vec2(0.0f);
	float maxCameraX = std::max(0.0f, validLevelSize.x - m_camera.getViewportWidth());
	float maxCameraY = std::max(0.0f, validLevelSize.y - m_camera.getViewportHeight());

	m_camera.setMaxCameraX(maxCameraX);
	m_camera.setMaxCameraY(maxCameraY);

	m_physicsMgr.onAttach(&m_scene, &world);

}
void GameManager::handleInput(const InputManager& input)
{
	m_playerInputSystem.update(m_scene, input);
}
void GameManager::render(Renderer2D& renderer, Atlas& atlas, double alpha)
{ 
	m_rendererMgr.renderScene(m_scene, renderer, atlas, alpha);
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
		float camX = std::clamp(p_transform->position.x - m_camera.getViewportWidth() * 0.5f, 0.0f, m_camera.getMaxCameraX());
		float camY = std::clamp(p_transform->position.y - m_camera.getViewportHeight() * 0.5f, 0.0f, m_camera.getMaxCameraY());
		m_camera.setPosition(glm::vec2(camX, camY));
	}
	else
	{
		return;
	}
}
	
