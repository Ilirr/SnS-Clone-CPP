#pragma once
#include <string>
#include <functional>

// Forward declare avoid pulling GLFW/GL headers into others
struct GLFWwindow;

class Window
{

public: 
	Window(int width, int height, const char* title);
	~Window();

	bool isValid() const; 
	bool shouldClose() const;

	void setResizeCallback(const std::function<void(int, int)>& callback);
	void swapBuffers();
	void pollEvents();
	void processInput();
	bool isKeyDown(int key) const;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	int getHeight() const;
	int getWidth() const;

	const std::string& getTitle() const;

private:
	void resizeCallback(int width, int height);
	int m_width = 800, m_height = 600;
	GLFWwindow* m_window = nullptr;
	std::string m_title;
	std::function<void(int, int)> m_resizeCallback;

};
