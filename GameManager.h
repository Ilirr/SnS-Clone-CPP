#pragma once
#include "LevelLoader.h"
#include "Input.h"
#include "PhysicsSystem.h"
#include "Renderer2D.h"
#include "Atlas.h"
class GameManager
{
public:
	GameManager();
	void init();
	void update(double dt);
	void handleInput(const InputManager& input);
	void render(Renderer2D& renderer, Atlas& atlas, double alpha);
	// Expose entity/static queries for rendering
	const std::vector<Entity>& getEntities() const { return m_physicsMgr.getEntities(); }
	const std::vector<StaticBody>& getStaticBodies() const { return m_physicsMgr.getStaticBodies(); }

private:
	PhysicsSystem m_physicsMgr;
	int m_playerEntity = -1;
};
