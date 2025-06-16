#include "Application.h"

#include <iostream>
#include "events/KeyEvent.h"
#include "core/Input.h"

bool Application::StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen, bool vsync)
{
    p_window = new Window();
    if (!p_window->StartUp(title, width, height, fullscreen, vsync)) {
        std::cout << "Couldn't initialize window..." << std::endl;
        return false;
    }
    p_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    if (!m_layerStack.StartUp()) {
        std::cout << "Couldn't initialize layer stack..." << std::endl;
        return false;
    }
    m_layerStack.PushOverlay(&m_imGuiLayer);

    return true;
}

void Application::ShutDown()
{
    p_window->ShutDown();
    delete p_window;
}

void Application::PushLayer(Layer* layer)
{
    m_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
    m_layerStack.PushOverlay(layer);
}

void Application::PopLayer(Layer* layer)
{
    m_layerStack.PopLayer(layer);
}

void Application::PopOverlay(Layer* layer)
{
    m_layerStack.PopOverlay(layer);
}

void Application::Run()
{
    double lastTime = glfwGetTime();
    while (!p_window->ShouldClose()) {
        // delta time calculations
        double currentTime = glfwGetTime();
        double timestep = currentTime - lastTime;
        lastTime = currentTime;

        OnUpdate(timestep);
        OnRender();
        OnImGuiRender();

        p_window->PollEvents();
        p_window->Update();
    }
}

void Application::OnEvent(Event& event)
{
    if (event.GetEventType() == EventType::KeyPressed) {
        if (((KeyPressedEvent&)event).GetKeyCode() == GLFW_KEY_F11) {
            p_window->SetFullscreen(!p_window->IsFullscreen());
        }
    }
    for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
    {
        (*--it)->OnEvent(event);
        if (event.Handled)
            break;
    }
}

void Application::OnUpdate(double timestep)
{
    Input::OnUpdate();
    for (Layer* layer : m_layerStack) {
        layer->OnUpdate(timestep);
    }
}

void Application::OnRender()
{
    glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (Layer* layer : m_layerStack) {
        layer->OnRender();
    }
}

void Application::OnImGuiRender()
{
    m_imGuiLayer.Begin();
    for (Layer* layer : m_layerStack) {
        layer->OnImGuiRender();
    }
    m_imGuiLayer.End();
}
