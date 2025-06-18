#include "Window.h"

#include "events/Event.h"
#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "core/Log.h"

#include <iostream>

bool Window::StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen, bool vsync)
{
	m_windowData = {
		title,
		width, height,
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
	glfwDestroyWindow(p_window);
	glfwTerminate();
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::Update()
{
	glfwSwapBuffers(p_window);
}

void Window::SetVSync(bool value)
{
	m_windowData.VSync = value;
	if (value)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void Window::SetFullscreen(bool value)
{
	m_windowData.Fullscreen = value;
	const GLFWvidmode* mode = glfwGetVideoMode(p_monitor);
	if (m_windowData.Fullscreen) {
		glfwSetWindowMonitor(p_window, p_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		int xpos = (mode->width / m_windowData.Width) / 2;
		int ypos = (mode->height / m_windowData.Height) / 2 + 40;
		glfwSetWindowMonitor(p_window, NULL, xpos, ypos, m_windowData.WindowedWidth, m_windowData.WindowedHeight, 0);
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
		glfwSetWindowMonitor(window, NULL, xpos, ypos, windowData.WindowedWidth, windowData.WindowedHeight, 0);
	}
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}

	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	GLCall(glDepthMask(GL_TRUE));

	SET_DEBUG_CALLBACK;

	return true;
}

void Window::CreateCallbacks()
{
	glfwSetWindowCloseCallback(p_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.ShouldClose = true;
		});

	glfwSetWindowSizeCallback(p_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			if (!data.Fullscreen) {
				data.WindowedWidth = width;
				data.WindowedHeight = height;
			}
			glViewport(0, 0, data.Width, data.Height);
			WindowResizedEvent event(width, height);
			data.EventCallback(event);
		});

	glfwSetKeyCallback(p_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetMouseButtonCallback(p_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(p_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
}
