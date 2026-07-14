#include "WeaponSystem.h"
#include "Scene.h"

void WeaponSystem::update(Scene& scene, float deltaTime)
{
	// Iterate through active entities and update those that have a WeaponComponent
	for (const auto& entity : scene.getActiveEntities())
	{
		auto weapon = scene.getWeapon(entity);
		if (weapon)
		{
			updateWeaponState(*weapon, deltaTime);
			// update visual weapon entity to follow the owner
			auto ownerTransform = scene.getTransform(entity);
			if (weapon->weaponEntity.isValid() && ownerTransform)
			{
				auto wTransform = scene.getTransform(weapon->weaponEntity);
				if (wTransform)
				{
					// copy previous position
					wTransform->prevPosition = wTransform->position;
					// weapon size: half the owner's height (square)
					glm::vec2 weaponSize = glm::vec2(ownerTransform->size.y * 0.5f, ownerTransform->size.y * 0.5f);
					wTransform->size = weaponSize;
				// position depends on owner's facing (if owner's sprite is flipped horizontally, place on left)
				auto ownerSprite = scene.getSprite(entity);
				if (ownerSprite && ownerSprite->flipX)
				{
					// left-middle positioning
					wTransform->position.x = ownerTransform->position.x - (weaponSize.x * 0.5f);
				}
				else
				{
					// right-middle positioning (top-left anchor)
					wTransform->position.x = ownerTransform->position.x + ownerTransform->size.x - (weaponSize.x * 0.5f);
				}
				wTransform->position.y = ownerTransform->position.y + (ownerTransform->size.y * 0.5f) - (weaponSize.y * 0.5f);

				// Keep visual weapon sprite flipped to match owner
				auto wSprite = scene.getSprite(weapon->weaponEntity);
				if (wSprite && ownerSprite)
				{
					wSprite->flipX = ownerSprite->flipX;
				}
				}
			}
		}
	}
}
void WeaponSystem::updateWeaponState(WeaponComponent& weapon, float deltaTime)
{
	if (weapon.state.isAttacking)
	{
		weapon.state.attackTimer += deltaTime;
		if (weapon.state.attackTimer >= weapon.state.attackDuration)
		{
			weapon.state.isAttacking = false;
			weapon.state.attackTimer = 0.0f;
		}
	}
}