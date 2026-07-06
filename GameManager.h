#pragma once
#include "LevelLoader.h"
#include "Input.h"
#include "PhysicsSystem.h"

class GameManager
{
public:
	GameManager();
	void init();
	void update(double dt);
	void handleInput(const InputManager& input);
	// Expose entity/static queries for rendering
	const std::vector<Entity>& getEntities() const { return m_physics.getEntities(); }
	const std::vector<StaticBody>& getStaticBodies() const { return m_physics.getStaticBodies(); }

private:
	PhysicsSystem m_physics;
	int m_playerEntity = -1;
};
