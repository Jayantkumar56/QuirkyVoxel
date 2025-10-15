

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "GameLayer.h"

#include <iostream>


namespace Mct {

    Application::Application() noexcept {
        // Initialise the global GLFW state first.
        if (!Window::Initialise()) {
            std::cerr << "Failed to initialise GLFW!" << std::endl;
            return;
        }

        m_Window = Window::Create("Minecraft Clone", 1280, 720);
    }

    Application::~Application() {
        Window::Terminate(); // Clean up GLFW
    }

    void Application::Run() {
        // Run the loop only when the window was successfully created.
        if (!m_Window) {
            return;
        }

        PushLayer<GameLayer>();

        while (!m_Window->ShouldClose()) {
            m_Window->PollEvents();

            glClearColor(0.2f, 0.5f, 0.5f, 1.0f); // Teal color
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto& layer : m_LayerStack) {
                layer->OnUpdate();
            }

            m_Window->SwapBuffers();
        }
    }

}
