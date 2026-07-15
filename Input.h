#pragma once
#include <vector>
class Window;
class InputManager
{
public:
	explicit InputManager(Window* window);
	void update();
	bool isDown(int key) const;
	bool wasPressed(int key) const;
	bool wasReleased(int key) const;

private:
	Window* m_window = nullptr;
	static const int MaxKeys = 512;
	std::vector<char> m_prev;
	std::vector<char> m_curr;
};
