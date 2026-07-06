#include "Application.h"
#include <algorithm>
#include <iostream>
#include <chrono>
Application::Application()
{
	window = std::make_unique<Window>(800,600,"MyGame");
	// create input manager now that window exists
	m_input = std::make_unique<InputManager>(window.get());
	initGraphics();
}

void Application::update(double dt)
{
	m_physics.updateAll(dt);
}

void Application::render(double alpha)
{
	if (renderer && m_atlas)
	{
		// Draw static bodies first (debug colored quads)
		const auto& statics = m_physics.getStaticBodies();
		for (const auto& s : statics)
		{
			// draw as green debug quad
			renderer->drawQuad(s.transform.position, s.transform.size, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
		}

		const auto& ents = m_physics.getEntities();
		if (!ents.empty())
		{
			for (const auto& e : ents)
			{
				renderer->drawQuad(e.transform.position, e.transform.size, m_atlas->getSprite("hero"), color);
			}
		}
		else
		{
			renderer->drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(64.0f, 64.0f), m_atlas->getSprite("hero"), color);
		}
	}
}
void Application::initGraphics()
{
	m_atlas = std::make_unique<Atlas>("assets/textures/hero.png");
	m_atlas->addSprite("hero", 0, 0, 64, 64);
	renderer = std::make_unique<Renderer2D>();

	// Create a dynamic entity (AoS) and register with physics system
	Entity ent;
	ent.transform.position = glm::vec2(100.0f, 100.0f);
	ent.transform.size = glm::vec2(64.0f, 64.0f);
	ent.collider.size = ent.transform.size;
	ent.body.velocity = glm::vec2(50.0f, 0.0f); // px/s to the right
	ent.body.gravityScale = 1.0f;
	m_playerEntity = m_physics.addEntity(ent);

	// Add a ground static body so the dynamic entity can collide and rest on it
	StaticBody ground;
	ground.transform.position = glm::vec2(0.0f, 400.0f); // y position where ground starts
	ground.transform.size = glm::vec2(800.0f, 50.0f); // wide ground
	ground.collider.size = ground.transform.size;
	ground.collider.offset = glm::vec2(0.0f, 0.0f);
	m_physics.addStaticBody(ground);

	// Map out your sprites (you only do this once at startup!)
	// E.g., The hero is at pixel (0,0) and is 64x64 pixels big
	if (window && window->isValid())
	{
		// Forward window resize events to the renderer
		window->setResizeCallback([this](int w, int h) {
			if (renderer)
			{
				renderer->onResize(w, h);
			}
			});
		// Call once now to set initial projection/viewport
		if (renderer) renderer->onResize(window->getWidth(), window->getHeight());
	}

}
void Application::run()
{
	using clock = std::chrono::high_resolution_clock;
	auto previous = clock::now();
	double accumulator = 0.0;
	const double maxFrameTime = 0.25; // clamp to avoid spiral of death
	const double dt = m_fixedDelta;

	while (!window->shouldClose())
	{
		auto now = clock::now();
		double frameTime = std::chrono::duration<double>(now - previous).count();
		previous = now;
		if (frameTime > maxFrameTime)
		{
			frameTime = maxFrameTime;
		}
		accumulator += frameTime;

		window->processInput();
		m_input->update();

		Entity& p = m_physics.getEntity(m_playerEntity);
		const float moveSpeed = 150.0f; // px/s
		const float jumpImpulse = -1000.0f; // negative is up

		bool left = m_input->isDown(GLFW_KEY_A) || m_input->isDown(GLFW_KEY_LEFT);
		bool right = m_input->isDown(GLFW_KEY_D) || m_input->isDown(GLFW_KEY_RIGHT);

		if (left && !right) p.body.velocity.x = -moveSpeed;
		else if (right && !left) p.body.velocity.x = moveSpeed;
		else p.body.velocity.x = 0.0f;

		if (m_input->wasPressed(GLFW_KEY_SPACE) && p.body.isGrounded)
		{
			p.body.velocity.y = jumpImpulse;
			p.body.isGrounded = false;
		}
		while (accumulator >= dt)
		{
			update(dt);
			accumulator -= dt;
		}
		double alpha = (dt > 0.0) ? (accumulator / dt) : 0.0;

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (renderer)
		{
			renderer->begin();
			render(alpha);
			renderer->end();
		}
		window->swapBuffers();
		window->pollEvents();
	}
}