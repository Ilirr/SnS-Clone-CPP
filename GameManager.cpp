#include "GameManager.h"
#include "LevelLoader.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
GameManager::GameManager(){}
void GameManager::init()
{
	LevelLoader loader;

	m_scene.addListener(&m_physicsMgr);

	loader.loadLevel("assets/levels/levels1.txt", m_physicsMgr, m_scene, 64.0f);

}
void GameManager::handleInput(const InputManager& input)
{
	if (!m_playerEntity.isValid())
	{
		return;
	}

	EntityID& p = m_scene.getEntity(m_playerEntity);

	bool left = input.isDown(GLFW_KEY_A) || input.isDown(GLFW_KEY_LEFT);
	bool right = input.isDown(GLFW_KEY_D) || input.isDown(GLFW_KEY_RIGHT);

	if (left && !right) p.RigidbodyComponent->velocity.x = -p.RigidbodyComponent->moveSpeed;
	else if (right && !left) p.RigidbodyComponent->velocity.x = p.RigidbodyComponent->moveSpeed;
	else p.RigidbodyComponent->velocity.x = 0.0f;

	if (input.wasPressed(GLFW_KEY_SPACE) && p.RigidbodyComponent->isGrounded)
	{
		p.RigidbodyComponent->velocity.y = p.RigidbodyComponent->jumpImpulse;
		p.RigidbodyComponent->isGrounded = false;
	}

}
void GameManager::render(Renderer2D& renderer, Atlas& atlas, double alpha)
{
	m_rendererMgr.renderScene(m_scene, renderer, atlas, alpha);
}

void GameManager::update(double dt)
{
	m_camera.setPrevPosition(m_camera.getPosition());
	m_physicsMgr.updateAll(dt);

	if (m_playerEntity.isValid()) {
		Entity& p = m_scene.getEntity(m_playerEntity);
		float camX = p.TransformComponent.position.x - 400.0f;
		float camY = p.TransformComponent.position.y - 300.0f;
		m_camera.setPosition(glm::vec2(camX, camY));
	}
}
	
