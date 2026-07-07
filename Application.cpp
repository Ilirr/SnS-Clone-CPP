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
void Application::initGraphics()
{
	m_atlas = std::make_unique<Atlas>("assets/textures/hero.png");
	m_atlas->addSprite("hero", 0, 0, 64, 64);
	renderer = std::make_unique<Renderer2D>();

	if (window && window->isValid())
	{
		window->setResizeCallback([this](int w, int h) {
			if (renderer)
			{
				renderer->onResize(w, h);
			}
			});
		if (renderer)
		{
			renderer->onResize(window->getWidth(), window->getHeight());
		}
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
			m_game.render(*renderer, *m_atlas, alpha);
			renderer->end();
		}
		window->swapBuffers();
		window->pollEvents();
	}
}