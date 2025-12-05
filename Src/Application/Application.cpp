

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "Window.h"
#include "Layers/GameLayer.h"
#include "Events/EventDispatcher.h"
#include "World/Block/BlockDataManager.h"
#include "World/Biome/BiomeDataManager.h"
#include "Utils/Logger.h"
#include "Utils/Profiler.h"


namespace Mct {

    Application::Application() noexcept :
            m_LayerManager(this)
    {
        static EventCallbackFn eventCallback = +[] (Event& e, void* userData) {
            Application* app = static_cast<Application*>(userData);
            app->m_LayerManager.OnEvent(e);
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

        m_LayerManager.PushLayer<GameLayer>();

        while (!m_Window->ShouldClose()) {
            MCT_PROFILE_FRAME("MainThread");
            MCT_PROFILE_FUNCTION();

            const double time      = glfwGetTime();
            const float  deltaTime = static_cast<float>(time - m_LastFrameTime);

            m_LastFrameTime = time;

            m_LayerManager.Update();

            m_Window->PollEvents();
            m_LayerManager.UpdateLayers(deltaTime);
            m_Window->SwapBuffers();
        }

        m_LayerManager.Clear();
    }

}
