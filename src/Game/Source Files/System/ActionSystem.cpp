#include "ActionSystem.h"
#include "Scene.h"

void ActionSystem::update(Scene& scene, float deltaTime)
{
	(void)deltaTime;

	for (const auto& entity : scene.getActiveEntities())
	{
		auto intent = scene.getIntent(entity);
		if (!intent)
			continue;

		auto rigidbody = scene.getRigidbody(entity);
		if (rigidbody)
		{
			rigidbody->velocity.x = intent->moveDirection.x * rigidbody->moveSpeed;
			if (intent->moveDirection.x != 0.0f)
			{
				auto sprite = scene.getSprite(entity);
				if (sprite)
					sprite->flipX = intent->moveDirection.x < 0.0f;
			}

			if (intent->jump && rigidbody->isGrounded)
			{
				rigidbody->velocity.y = rigidbody->jumpImpulse;
				rigidbody->isGrounded = false;
			}
		}
		intent->jump = false;

		auto weapon = scene.getWeapon(entity);
		if (weapon && intent->attack && weapon->state == WeaponState::Idle)
		{
			weapon->state = WeaponState::Attacking;
			weapon->attackTimer = 0.0f;
		}
		intent->attack = false;
	}
}