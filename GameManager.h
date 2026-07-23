#pragma once
#include "PhysicsSystem.h"
#include "RendererSystem.h"
#include "Camera2D.h"
#include "Scene.h"
#include "WeaponSystem.h"
#include "WeaponRegistry.h"
#include "ActionSystem.h"
#include "PlayerInputSystem.h"


class InputManager;
class Renderer2D;
class Atlas;
class GameManager
{
public:
	GameManager();
	void init(Atlas& atlas);
	void update(double dt);
	void handleInput(const InputManager& input, Renderer2D& renderer);
	void render(Renderer2D& renderer, Atlas& atlas, double alpha);

	Camera2D& getCamera() { return m_camera; }


private:
	CollisionWorld m_collisionWorld;
	PhysicsSystem m_physicsMgr;
	RendererSystem m_rendererMgr;
	WeaponSystem m_weaponMgr;
	WeaponRegistry m_weaponRegistry;
	Scene m_scene;
	ActionSystem m_actionMgr;
	PlayerInputSystem m_playerInputSystem;
	Camera2D m_camera;
	bool m_showDebugOverlay = false;
};
