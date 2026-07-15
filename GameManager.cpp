#include "GameManager.h"
#include "LevelLoader.h"
#include "Input.h"
#include "WeaponRegistry.h"
#include "Atlas.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GameManager::GameManager(){}
void GameManager::init(Atlas& atlas)
{
	LevelLoader loader;

	m_scene.addListener(&m_physicsMgr);

	// initialize weapon definitions (resolve sprite IDs using the atlas)
	WeaponRegistry::init(atlas);

	loader.loadLevel("assets/levels/levels1.txt", m_scene, atlas, 64.0f);

}
void GameManager::handleInput(const InputManager& input)
{
	EntityID p = m_scene.getPlayerEntity();
	if (!p.isValid())
	{
		return;
	}
	if (input.wasPressed(GLFW_KEY_E))
	{
		//E is attack button, so we set the intent to attack
		auto intent = m_scene.getIntent(p);
		if (intent) intent->attack = true;
	}


	if (input.wasPressed(GLFW_KEY_F))
	{
		std::cout << "Player components:\n";
		auto &mgr = m_scene.getEntityManager();
		if (mgr.getTransform(p)) std::cout << " - TransformComponent\n";
		if (mgr.getRigidbody(p)) std::cout << " - RigidbodyComponent\n";
		if (mgr.getSprite(p)) std::cout << " - SpriteComponent\n";
		if (mgr.getCollider(p)) std::cout << " - ColliderComponent\n";
		if (mgr.getTag(p)) std::cout << " - TagComponent\n";
		if (mgr.getWeapon(p))
		{
			std::cout << " - WeaponComponent\n";

			auto weapon = mgr.getWeapon(p);
			weapon->type == WeaponType::Sword ? std::cout << "   - WeaponType: Sword\n" : std::cout << "   - WeaponType: Other\n";

			auto weaponEntity = weapon->weaponEntity;
			ColliderComponent* weaponCollider = mgr.getCollider(weaponEntity);
			
			if (weaponCollider)
			{
				std::cout << "   - Weapon has ColliderComponent\n";
			}
			else
			{
				std::cout << "   - Weapon does NOT have ColliderComponent\n";
			}

		}
			
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

		auto sprite = m_scene.getSprite(p);

		if (sprite) sprite->flipX = true;
	}
	else if (right && !left)
	{
		p_rigidBody->velocity.x = p_rigidBody->moveSpeed;

		auto sprite = m_scene.getSprite(p);

		if (sprite) sprite->flipX = false;
	}
	else
	{
		p_rigidBody->velocity.x = 0.0f;
	}
	if (input.wasPressed(GLFW_KEY_SPACE) && p_rigidBody->isGrounded)
	{
		auto intent = m_scene.getIntent(p);
		intent->jump = true;
	}

}
void GameManager::render(Renderer2D& renderer, Atlas& atlas, double alpha)
{ 
	m_rendererMgr.renderScene(m_scene, renderer, atlas, alpha);
}

void GameManager::update(double dt)
{
	m_camera.setPrevPosition(m_camera.getPosition());
	// Process actions (input intents) before physics so impulses are applied in the same frame
	m_actionMgr.update(m_scene, static_cast<float>(dt));
	m_physicsMgr.updateAll(dt);
	m_weaponMgr.update(m_scene, static_cast<float>(dt)); //static cast because updateAll takes a float, but dt is a double and it doesnt need to be exact

	const EntityID p = m_scene.getPlayerEntity();
	auto p_transform = m_scene.getTransform(p);

	if (m_scene.getEntityManager().isEntityAlive(p) && p_transform)
	{
		float camX = p_transform->position.x - 400.0f;
		float camY = p_transform->position.y - 300.0f;
		m_camera.setPosition(glm::vec2(camX, camY));
		
	}
}
	
