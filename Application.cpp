#include "Application.h"
#include "GameManager.h"
#include "Atlas.h"
#include "Renderer2D.h"
#include "Input.h"
#include "Window.h"
#include <algorithm>
#include <iostream>
#include <chrono>
Application::Application()
{
	window = std::make_unique<Window>(800,600,"MyGame");

	if (window && !window->isValid())
	{
		std::cerr << "Failed to create window!" << std::endl;
		exit(EXIT_FAILURE);
	}
	m_input = std::make_unique<InputManager>(window.get());
	m_game = std::make_unique<GameManager>();

	initGraphics();
	m_game->init(*m_atlas);
}
Application::~Application(){}
void Application::initGraphics()
{
	renderer = std::make_unique<Renderer2D>();
	m_atlas = std::make_unique<Atlas>("assets/levels/moon.png");
	m_atlas->addSprite(
		"background",
		0.0f,
		0.0f,
		static_cast<float>(m_atlas->getTexture().getWidth()),
		static_cast<float>(m_atlas->getTexture().getHeight()));
	m_atlas->addSpriteFromTexture("hero", "assets/levels/Hero.gif", 40.0f, 40.0f, 20.0f, 38.0f);
	renderer->setCamera(&m_game->getCamera());
	
	window->setResizeCallback([this](int w, int h)
	{
		renderer->onResize(w, h);
	});
	renderer->onResize(window->getWidth(), window->getHeight());
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

		window->pollEvents();
		window->processInput();
		m_input->update();
		m_game->handleInput(*m_input, *renderer);


		while (accumulator >= dt)
		{
			m_game->update(dt);
			accumulator -= dt;
		}
		double alpha = (dt > 0.0) ? (accumulator / dt) : 0.0;

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (renderer)
		{
			renderer->begin(alpha);
			m_game->render(*renderer, *m_atlas, alpha);
			renderer->end();

		}
		window->swapBuffers();

	}
}