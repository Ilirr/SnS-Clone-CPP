#pragma once
#include "Components.h"
#include "Scene.h"
class WeaponSystem
{
	public:

	void updateWeaponState(WeaponComponent& weapon, float deltaTime);
	void update(Scene& scene, float deltaTime);

};