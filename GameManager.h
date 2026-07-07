#pragma once
#include "PhysicsSystem.h"
#include "RendererSystem.h"
#include "Camera2D.h"

// Forward declarations to reduce header coupling
class InputManager;
class Renderer2D;
class Atlas;
class LevelLoader;
class GameManager
{
public:
	GameManager();
	void init();
	void update(double dt);
	void handleInput(const InputManager& input);
	void render(Renderer2D& renderer, Atlas& atlas, double alpha);

	// Access to the camera for external wiring
	Camera2D& getCamera() { return m_camera; }


private:
	PhysicsSystem m_physicsMgr;
	RendererSystem m_rendererMgr;
	// Camera to control view; renderer can be given a pointer to this camera
	Camera2D m_camera;

	int m_playerEntity = -1;
};
