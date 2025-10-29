

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "Window.h"
#include "GameLayer.h"
#include "Events/EventDispatcher.h"
#include "ImguiContext.h"
#include "Events/MouseEvents.h"

#include <iostream>


namespace Mct {

    Application::Application() noexcept {
        // Initialise the global GLFW state first.
        if (!Window::Initialise()) {
            std::cerr << "Failed to initialise GLFW!" << std::endl;
            return;
        }

        EventCallbackFn eventCallback = [] (Event& e, void* userData) {
            Application* app = static_cast<Application*>(userData);
            app->OnEvent(e);
        };

        EventDispatcher::RegisterEventCallback(eventCallback, this);

        m_Window = Window::Create("Minecraft Clone", 1280, 720, ImguiContext::GetEventCallbacks());

        if (m_Window) {
            ImguiContext::Init(m_Window->GetHandle());
        }
    }

    Application::~Application() {
        Window::Terminate(); // Clean up GLFW
        ImguiContext::Terminate();
    }

    void Application::Run() {
        // Run the loop only when the window was successfully created.
        if (!m_Window) {
            return;
        }

        PushLayer<GameLayer>();

        while (!m_Window->ShouldClose()) {
            m_Window->PollEvents();

            glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImguiContext::Begin();

            for (const auto& layer : m_LayerStack) {
                layer->OnUpdate();
            }

            ImguiContext::End();

            m_Window->SwapBuffers();
        }
    }

    void Application::OnEvent(Event& e) {
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (e.IsHandled())
                break;

            (*it)->OnEvent(e);
        }
    }

}
