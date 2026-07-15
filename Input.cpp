#include "Input.h"
#include "Window.h"
InputManager::InputManager(Window* window)
	: m_window(window), m_prev(MaxKeys), m_curr(MaxKeys)
{
}

void InputManager::update()
{
	m_prev = m_curr;
	for (int k = 0; k < MaxKeys; ++k)
	{
		if (m_window)
			m_curr[k] = m_window->isKeyDown(k) ? 1 : 0;
		else
			m_curr[k] = 0;
	}
}

bool InputManager::isDown(int key) const
{
	if (key < 0 || key >= MaxKeys) return false;
	return m_curr[key];
}

bool InputManager::wasPressed(int key) const
{
	if (key < 0 || key >= MaxKeys) return false;
	return m_curr[key] && !m_prev[key];
}

bool InputManager::wasReleased(int key) const
{
	if (key < 0 || key >= MaxKeys) return false;
	return !m_curr[key] && m_prev[key];
}
