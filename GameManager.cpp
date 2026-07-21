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

	m_scene.addListener(&m_physicsMgr);

	m_weaponRegistry.init(atlas);

	EntityID backgroundEntity = m_scene.createEntity();
	TransformComponent backgroundTransform;
	backgroundTransform.size = { 320.0f, 180.0f };
	backgroundTransform.prevPosition = backgroundTransform.position;
	m_backgroundSize = backgroundTransform.size;

	SpriteComponent backgroundSprite;
	backgroundSprite.spriteID = atlas.getSpriteId("background");

	m_scene.getEntityManager().addComponent(backgroundEntity, backgroundTransform);
	m_scene.getEntityManager().addComponent(backgroundEntity, backgroundSprite);

	const glm::vec2 levelSize = loader.loadLevel("assets/levels/final.ldtk", m_scene, atlas, m_weaponRegistry, 16.0f);
	if (levelSize.x > 0.0f && levelSize.y > 0.0f)
	{
		m_backgroundSize = levelSize;
		backgroundTransform.size = levelSize;
		m_scene.getEntityManager().addComponent(backgroundEntity, backgroundTransform);
	}
	float maxCameraX = std::max(0.0f, m_backgroundSize.x - m_camera.getViewportWidth());
	float maxCameraY = std::max(0.0f, m_backgroundSize.y - m_camera.getViewportHeight());

	m_camera.setMaxCameraX(maxCameraX);
	m_camera.setMaxCameraY(maxCameraY);

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

		float camX = std::clamp(p_transform->position.x - m_camera.getViewportWidth() * 0.5f, 0.0f, m_camera.getMaxCameraX());
		float camY = std::clamp(p_transform->position.y - m_camera.getViewportHeight() * 0.5f, 0.0f, m_camera.getMaxCameraY());
		m_camera.setPosition(glm::vec2(camX, camY));
		
	}
	else
	{
		return;
	}
}
	
