#include "Application.h"

#include <iostream>

bool Application::StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen, bool vsync)
{
    p_window = new Window();
    if (!p_window->StartUp(title, width, height, fullscreen, vsync)) {
        std::cout << "Couldn't initialize window..." << std::endl;
        return false;
    }
    return true;
}

void Application::ShutDown()
{
    p_window->ShutDown();
    delete p_window;
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
}

void Application::OnUpdate(double timestep)
{
}

void Application::OnRender()
{
    glClearColor(0.2f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Application::OnImGuiRender()
{
}
