#pragma once
#include "Window.h"
#include "Renderer2D.h"
#include "glm/glm.hpp"
#include "memory"
#include "Atlas.h"
#include "Input.h"
#include "GameManager.h"

class Application
{

public:
	Application();
	~Application() = default;
	void run();
	void update(double dt);
	void render(double alpha);


private:
	void initGraphics();
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer2D> renderer;
	std::unique_ptr<Texture> m_sprite;
	glm::vec4 color = glm::vec4(1.0f);

	std::unique_ptr<Atlas> m_atlas;

	GameManager m_game;
	std::unique_ptr<InputManager> m_input;
	const double m_fixedDelta = 1.0 / 60.0; 




}; 