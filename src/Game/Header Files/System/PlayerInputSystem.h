#pragma once

class InputManager;
class Scene;

class PlayerInputSystem
{
public:
	void update(Scene& scene, const InputManager& input) const;
};
