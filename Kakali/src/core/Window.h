#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "events/Event.h"

struct WindowData {
	const char* Title;
	uint32_t Width, Height;
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

	inline void SetVSync(bool value);
	inline const bool IsVSync() const { return m_windowData.VSync; }

	inline void SetFullscreen(bool value);
	inline const bool IsFullscreen() const { return m_windowData.Fullscreen; }

	inline const bool ShouldClose() const { return m_windowData.ShouldClose; }

	inline WindowData& GetWindowData() { return m_windowData; }
public:
	static void SetGlobalVSync(bool value);
	static bool IsGlobalVSync();

	static void SetGlobalFullscreen(bool value);
	static bool IsGlobalFullscreen();

	static WindowData& GetGlobalWindowData();
private:
	bool StartGLFW();
	bool CreateWindow();
	bool SetupMonitor();
	bool StartGLAD();
	void CreateCallbacks();
private:
	GLFWwindow* p_window;
	GLFWmonitor* p_monitor;

	WindowData m_windowData;
};

