

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "Window.h"
#include "GameLayer.h"
#include "Events/EventDispatcher.h"
#include "ImguiContext.h"
#include "World/Block/BlockDataManager.h"
#include "World/Biome/BiomeDataManager.h"
#include "Utils/Logger.h"
#include "Utils/Profiler.h"


namespace Mct {

    Application::Application() noexcept {
        static EventCallbackFn eventCallback = +[] (Event& e, void* userData) {
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
        ImguiContext::Terminate();
    }

    void Application::Run() {
        // Run the loop only when the window was successfully created.
        if (!m_Window) {
            return;
        }

        MCT_INFO("Application is running.");

        BlockDataManager::Init();
        BiomeDataManager::Init();

        PushLayer<GameLayer>();

        while (!m_Window->ShouldClose()) {
            MCT_PROFILE_FRAME("MainThread");
            MCT_PROFILE_FUNCTION();

            const double time      = glfwGetTime();
            const float  deltaTime = static_cast<float>(time - m_LastFrameTime);

            m_LastFrameTime = time;

            m_Window->PollEvents();

            ImguiContext::Begin();

            for (const auto& layer : m_LayerStack) {
                layer->OnUpdate(deltaTime);
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
