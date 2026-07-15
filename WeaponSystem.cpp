#include "WeaponSystem.h"
#include "Scene.h"

void WeaponSystem::update(Scene& scene, float deltaTime)
{
	// This makes weapon follow the owner entity's position and size, and updates the weapon's state based on its current state and the elapsed time.
	for (const auto& entity : scene.getActiveEntities())
	{
		auto weapon = scene.getWeapon(entity);
		if (!weapon) continue;

		updateWeaponState(*weapon, deltaTime);

		auto ownerTransform = scene.getTransform(entity);

		if (!weapon->weaponEntity.isValid() || !ownerTransform) continue;

		auto weaponTransform = scene.getTransform(weapon->weaponEntity);

		if (!weaponTransform) continue;

		weaponTransform->prevPosition = weaponTransform->position;

		glm::vec2 weaponSize = glm::vec2(ownerTransform->size.y * 0.5f, ownerTransform->size.y * 0.5f);

		weaponTransform->size = weaponSize;

		auto ownerSprite = scene.getSprite(entity);
		auto wSprite = scene.getSprite(weapon->weaponEntity);
		bool isFlipped = (ownerSprite && ownerSprite->flipX);

		if (isFlipped)
		{
			weaponTransform->position.x = ownerTransform->position.x - (weaponSize.x * 0.5f);
		}
		else
		{
			weaponTransform->position.x = ownerTransform->position.x + ownerTransform->size.x - (weaponSize.x * 0.5f);
		}
		weaponTransform->position.y = ownerTransform->position.y + (ownerTransform->size.y * 0.5f) - (weaponSize.y * 0.5f);

		if (wSprite && ownerSprite)
		{
			wSprite->flipX = ownerSprite->flipX;
		}
	}
}
void WeaponSystem::updateWeaponState(WeaponComponent& weapon, float deltaTime)
{

	switch (weapon.state)
	{
		case WeaponState::Idle:
			break;

		case WeaponState::Attacking:
			weapon.attackTimer += deltaTime;
			if (weapon.attackTimer >= weapon.attackDuration)
			{
				weapon.state = WeaponState::Idle;
				weapon.attackTimer = 0.0f;
			}
			break;
	}
}