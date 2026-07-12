#include "WeaponSystem.h"

void WeaponSystem::update(Scene& scene, float deltaTime)
{
	// Iterate through active entities and update those that have a WeaponComponent
	for (const auto& entity : scene.getActiveEntities())
	{
		auto weapon = scene.getWeapon(entity);
		if (weapon)
		{
			updateWeaponState(*weapon, deltaTime);
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