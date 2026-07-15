#include "ActionSystem.h"
#include "Scene.h"
#include <iostream>

void ActionSystem::update(Scene& scene, float deltaTime)
{
	for (const auto& entity : scene.getActiveEntities())
	{
		auto intent = scene.getIntent(entity);
		auto weapon = scene.getWeapon(entity);

		if (!intent || !weapon)
			continue;
		if (intent->attack)
		{
			weapon->state = WeaponState::Attacking;
			// print out the weapon state of the entity to the console
			std::cout << "Entity " << entity.index << " is attacking with weapon type " << static_cast<int>(weapon->type) << std::endl;
			//print out weaponState of the entity to the console
			std::cout << "Weapon state: " << static_cast<int>(weapon->state) << std::endl;
			intent->attack = false;
		}
		else
		{
			weapon->state = WeaponState::Idle;
		}
		if (intent->jump)
		{
			auto rigidbody = scene.getRigidbody(entity);

			if (rigidbody)
			{
				if (rigidbody->isGrounded)
				{
					rigidbody->velocity.y = rigidbody->jumpImpulse;
					rigidbody->isGrounded = false;
				}
				intent->jump = false;
			}
		}
	}
}