#pragma once

#include "Window.h"

#include "layers/LayerStack.h"
#include "layers/ImGuiLayer.h"
#include <queue>

class Application
{
public:
	Application() = default;
	~Application() = default;

	bool StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen = false, bool vsync = false);
	void ShutDown();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* layer);

	void Run();

	void QueueEvent(std::function<void(Event*)>&& func);
private:
	void OnEvent();
	void OnUpdate(double timestep);
	void OnRender();
	void OnImGuiRender();
private:
	Window m_window;
	LayerStack m_layerStack;
	ImGuiLayer m_imGuiLayer;

	std::queue<std::function<void(Event*)>> m_eventQueue;

	char m_eventBuffer[16];
	Event* p_eventBufferPointer;
};

