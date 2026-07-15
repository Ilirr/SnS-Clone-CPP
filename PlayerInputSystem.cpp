#include "PlayerInputSystem.h"
#include "Input.h"
#include "Scene.h"
#include <GLFW/glfw3.h>

void PlayerInputSystem::update(Scene& scene, const InputManager& input) const
{
	const EntityID player = scene.getPlayerEntity();
	auto intent = scene.getIntent(player);
	if (!intent)
	{
		return;
	}

	const bool left = input.isDown(GLFW_KEY_A) || input.isDown(GLFW_KEY_LEFT);
	const bool right = input.isDown(GLFW_KEY_D) || input.isDown(GLFW_KEY_RIGHT);
	intent->moveDirection.x = left == right ? 0.0f : (left ? -1.0f : 1.0f);

	if (input.wasPressed(GLFW_KEY_SPACE))
	{
		intent->jump = true;
	}

	if (input.wasPressed(GLFW_KEY_E))
	{
		intent->attack = true;
	}
}
