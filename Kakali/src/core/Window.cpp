#include "Window.h"

#include <iostream>

bool Window::StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen, bool vsync)
{
	m_windowData = {
		title,
		width, height,
		fullscreen, vsync
	};
	if (!StartGLFW()) {
		std::cout << "Couldn't initialize GLFW..." << std::endl;
		return false;
	}
	if (!CreateWindow()) {
		std::cout << "Couldn't create window..." << std::endl;
		return false;
	}
	if (!SetupMonitor()) {
		std::cout << "Couldn't initialize primary monitor..." << std::endl;
		return false;
	}
	if (!StartGLAD()) {
		std::cout << "Couldn't initialize GLAD..." << std::endl;
		return false;
	}
	CreateCallbacks();
	return true;
}

void Window::ShutDown()
{
	// TODO:
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::Update()
{
	glfwSwapBuffers(p_window);
}

inline void Window::SetVSync(bool value)
{
	m_windowData.VSync = value;
	if (value)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

inline void Window::SetFullscreen(bool value)
{
	m_windowData.Fullscreen = value;
	const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
	if (m_windowData.Fullscreen) {
		glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		int xpos = (mode->width / m_windowData.Width) / 2;
		int ypos = (mode->height / m_windowData.Height) / 2 + 40;
		glfwSetWindowMonitor(p_window, NULL, xpos, ypos, m_windowData.Width, m_windowData.Height, 0);
	}
}

void Window::SetGlobalVSync(bool value)
{

	(*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).VSync = value;
	if (value)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

bool Window::IsGlobalVSync()
{
	return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).VSync;
}

void Window::SetGlobalFullscreen(bool value)
{
	WindowData& windowData = Window::GetGlobalWindowData();
	windowData.Fullscreen = value;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwGetCurrentContext();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (windowData.Fullscreen) {
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		int xpos = (mode->width / windowData.Width) / 2;
		int ypos = (mode->height / windowData.Height) / 2 + 40;
		glfwSetWindowMonitor(window, NULL, xpos, ypos, windowData.Width, windowData.Height, 0);
	}
}

bool Window::IsGlobalFullscreen()
{
	return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).Fullscreen;
}

WindowData& Window::GetGlobalWindowData()
{
	return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext()));
}

bool Window::StartGLFW()
{
	return glfwInit() == GLFW_TRUE;
}

bool Window::CreateWindow()
{
	p_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title, NULL, NULL);
	if (!p_window)
	{
		return p_window;
	}

	glfwMakeContextCurrent(p_window);
	glfwSetWindowUserPointer(p_window, &m_windowData);
	return p_window;
}

bool Window::SetupMonitor()
{
	p_monitor = glfwGetPrimaryMonitor();
	if (!p_monitor) {
		return p_monitor;
	}

	if (m_windowData.Fullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
		m_windowData.Width = mode->width;
		m_windowData.Height = mode->height;
		glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		glViewport(0, 0, mode->width, mode->height);
	}
	return p_monitor;
}

bool Window::StartGLAD()
{
	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Window::CreateCallbacks()
{
	glfwSetWindowCloseCallback(p_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.ShouldClose = true;
		});
}
