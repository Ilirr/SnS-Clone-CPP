#pragma once
#include "LevelLoader.h"
#include "Input.h"
#include "PhysicsSystem.h"
#include "Renderer2D.h"
#include "Atlas.h"
#include "RendererSystem.h"
class GameManager
{
public:
	GameManager();
	void init();
	void update(double dt);
	void handleInput(const InputManager& input);
	void render(Renderer2D& renderer, Atlas& atlas, double alpha);


private:
	PhysicsSystem m_physicsMgr;
	RendererSystem m_rendererMgr;

	int m_playerEntity = -1;
};
