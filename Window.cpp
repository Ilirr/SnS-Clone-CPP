#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.h"
Window::Window(int width, int height, const char* title) : m_width(width), m_height(height), m_title(title), m_window(nullptr)
{
	if (!glfwInit())
	{

		std::cerr << "Failed to initialize GLFW\n";
		return;
	}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
	if (!m_window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return;
	}
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, Window::framebuffer_size_callback);

	glfwMakeContextCurrent(m_window);

    // initialize GL function pointers via GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        return;
    }

    // set the initial viewport to the requested size
    glViewport(0, 0, m_width, m_height);
}

bool Window::isValid() const
{
    return m_window != nullptr;
}
Window::~Window()
{
    if (m_window) {
        glfwDestroyWindow(m_window);
    }

    glfwTerminate();
}
bool Window::shouldClose() const
{
    return !m_window || glfwWindowShouldClose(m_window);
}
void Window::swapBuffers() 
{
    if (m_window) {
        glfwSwapBuffers(m_window);
    }
}
void Window::pollEvents() 
{
    glfwPollEvents();
}

int Window::getHeight() const
{
    return m_height;
}
int Window::getWidth() const {
    return m_width;

}
void Window::processInput() 
{
    if (m_window && glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

bool Window::isKeyDown(int key) const
{
    if (!m_window) return false;
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}
void Window::framebuffer_size_callback(GLFWwindow* glfwWindow, int width, int height)
{
    if (!glfwWindow) return;
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (win) {
        win->m_width = width;
        win->m_height = height;
        win->resizeCallback(width, height);
    }
}
void Window::resizeCallback(int width, int height)
{
    m_width = width;
    m_height = height;
    if (m_resizeCallback) m_resizeCallback(width, height);
}
void Window::setResizeCallback(const std::function<void(int, int)>& callback)
{
    m_resizeCallback = callback;
}



