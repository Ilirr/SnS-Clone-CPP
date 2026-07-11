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
	EntityID p = m_scene.getPlayerEntity();
	if (!p.isValid())
	{
		return;
	}

	bool left = input.isDown(GLFW_KEY_A) || input.isDown(GLFW_KEY_LEFT);
	bool right = input.isDown(GLFW_KEY_D) || input.isDown(GLFW_KEY_RIGHT);

	auto p_rigidBody = m_scene.getRigidbody(p);
	if (!p_rigidBody)
	{
		return;
	}

	if (left && !right)
	{
		p_rigidBody->velocity.x = -p_rigidBody->moveSpeed;
	}
	else if (right && !left)
	{
		p_rigidBody->velocity.x = p_rigidBody->moveSpeed;
	}
	else
	{
		p_rigidBody->velocity.x = 0.0f;
	}
	if (input.wasPressed(GLFW_KEY_SPACE) && p_rigidBody->isGrounded)
	{
		p_rigidBody->velocity.y = p_rigidBody->jumpImpulse;
		p_rigidBody->isGrounded = false;
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

	const EntityID p = m_scene.getPlayerEntity();
	if (p.isValid()) {
		auto p_transform = m_scene.getTransform(p);
		if (p_transform) 
		{
			float camX = p_transform->position.x - 400.0f;
			float camY = p_transform->position.y - 300.0f;
			m_camera.setPosition(glm::vec2(camX, camY));
		}
	}
}
	
