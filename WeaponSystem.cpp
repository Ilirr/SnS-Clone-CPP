#include "WeaponSystem.h"
#include "Scene.h"

void WeaponSystem::update(Scene& scene, float deltaTime)
{
	for (const auto& entity : scene.getActiveEntities())
	{
		auto weapon = scene.getWeapon(entity);
		if (!weapon) continue;

		updateWeaponState(*weapon, deltaTime);

		auto ownerTransform = scene.getTransform(entity);

		if (!weapon->weaponEntity.isValid() || !ownerTransform) return;

		auto wTransform = scene.getTransform(weapon->weaponEntity);

		if (!wTransform) return;

		wTransform->prevPosition = wTransform->position;

		glm::vec2 weaponSize = glm::vec2(ownerTransform->size.y * 0.5f, ownerTransform->size.y * 0.5f);

		wTransform->size = weaponSize;

		auto ownerSprite = scene.getSprite(entity);
		auto wSprite = scene.getSprite(weapon->weaponEntity);
		bool isFlipped = (ownerSprite && ownerSprite->flipX);

		if (isFlipped)
		{
			wTransform->position.x = ownerTransform->position.x - (weaponSize.x * 0.5f);
		}
		else
		{
			wTransform->position.x = ownerTransform->position.x + ownerTransform->size.x - (weaponSize.x * 0.5f);
		}
		wTransform->position.y = ownerTransform->position.y + (ownerTransform->size.y * 0.5f) - (weaponSize.y * 0.5f);

		// Match weapon sprite flip to owner

		if (wSprite && ownerSprite)
		{
			wSprite->flipX = ownerSprite->flipX;
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