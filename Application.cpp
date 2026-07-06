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
	// initialize game manager (loads level, registers entities)
	m_game.init();
}
void Application::update(double dt)
{

}

void Application::render(double alpha)
{
	if (renderer && m_atlas)
	{
		// Draw static bodies first (debug colored quads)
		const auto& statics = m_game.getStaticBodies();
		for (const auto& s : statics)
		{
			// draw as green debug quad
			renderer->drawQuad(s.transform.position, s.transform.size, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
		}
		const auto& ents = m_game.getEntities();
		if (!ents.empty())
		{
			for (const auto& e : ents)
			{
				renderer->drawQuad(e.transform.position, e.transform.size, m_atlas->getSprite("hero"), color);
			}
		}
	}
}
void Application::initGraphics()
{
	m_atlas = std::make_unique<Atlas>("assets/textures/hero.png");
	m_atlas->addSprite("hero", 0, 0, 64, 64);
	renderer = std::make_unique<Renderer2D>();

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
		m_game.handleInput(*m_input);
		while (accumulator >= dt)
		{
			m_game.update(dt);
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