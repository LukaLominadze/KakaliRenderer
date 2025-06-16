#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "events/Event.h"

struct WindowData {
	const char* Title;
	uint32_t Width, Height;
	uint32_t WindowedWidth, WindowedHeight;
	bool Fullscreen, VSync;
	bool ShouldClose;

	std::function<void(Event&)> EventCallback;
};

class Window
{
public:
	Window() = default;
	~Window() = default;

	bool StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen = false, bool vsync = false);
	void ShutDown();

	inline void SetEventCallback(const std::function<void(Event&)>& callback) {
		m_windowData.EventCallback = callback;
	}

	void PollEvents();
	void Update();

	void SetVSync(bool value);
	inline const bool IsVSync() const { return m_windowData.VSync; }

	void SetFullscreen(bool value);
	inline const bool IsFullscreen() const { return m_windowData.Fullscreen; }

	inline const bool ShouldClose() const { return m_windowData.ShouldClose; }

	inline WindowData& GetWindowData() { return m_windowData; }
public:
	static void SetGlobalVSync(bool value);
	inline static bool IsGlobalVSync() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).VSync; }

	static void SetGlobalFullscreen(bool value);
	inline static bool IsGlobalFullscreen() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).Fullscreen; }

	inline static WindowData& GetGlobalWindowData() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())); }
private:
	bool StartGLFW();
	bool CreateWindow();
	bool SetupMonitor();
	bool StartGLAD();
	void CreateCallbacks();
private:
	GLFWwindow* p_window = nullptr;
	GLFWmonitor* p_monitor = nullptr;

	WindowData m_windowData;
};

