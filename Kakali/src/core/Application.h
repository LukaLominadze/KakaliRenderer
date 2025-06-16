#pragma once

#include "Window.h"

class Application
{
public:
	Application() = default;
	~Application() = default;

	bool StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen = false, bool vsync = false);
	void ShutDown();

	void Run();
private:
	void OnEvent(Event& event);
	void OnUpdate(double timestep);
	void OnRender();
	void OnImGuiRender();
private:
	Window* p_window;
};

