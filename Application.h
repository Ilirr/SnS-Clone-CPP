#pragma once
#include <memory>
#include <glm/glm.hpp>
// Forward declarations to reduce header dependencies
class Window;
class Renderer2D;
class Texture;
class InputManager;
class Atlas;
class GameManager;
class Application
{

public:
	Application();
	~Application();
	void run();
	void update(double dt);

private:
	void initGraphics();
	std::unique_ptr<Window> window;
	std::unique_ptr<InputManager> m_input;
	std::unique_ptr<Renderer2D> renderer;
	std::unique_ptr<Atlas> m_atlas;
	std::unique_ptr<Texture> m_sprite;
	glm::vec4 color = glm::vec4(1.0f);


	std::unique_ptr<GameManager> m_game;
	const double m_fixedDelta = 1.0 / 60.0; 


}; 