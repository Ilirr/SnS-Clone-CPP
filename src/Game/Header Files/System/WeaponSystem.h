#pragma once
#include "../Registry/Components.h"
#include "../Game/Scene.h"
class WeaponSystem
{
	public:

	void updateWeaponState(WeaponComponent& weapon, float deltaTime);
	void update(Scene& scene, float deltaTime);

};