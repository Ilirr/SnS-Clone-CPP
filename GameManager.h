#pragma once
#include "PhysicsSystem.h"
#include "RendererSystem.h"
#include "Camera2D.h"
#include "Scene.h"


class InputManager;
class Renderer2D;
class Atlas;
class GameManager
{
public:
	GameManager();
	void init(Atlas& atlas);
	void update(double dt);
	void handleInput(const InputManager& input);
	void render(Renderer2D& renderer, Atlas& atlas, double alpha);

	Camera2D& getCamera() { return m_camera; }


private:
	PhysicsSystem m_physicsMgr;
	RendererSystem m_rendererMgr;
	Scene m_scene;

	Camera2D m_camera;

};
