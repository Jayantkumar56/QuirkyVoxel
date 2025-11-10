

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Layer.h"

#include <vector>
#include <memory>


namespace Mct {

    class Event;
    class Window;

    class Application {
    public:
        Application() noexcept;
        ~Application();

        void Run();

        template<typename T, typename... Args>
        requires (std::derived_from<T, Layer>&& std::constructible_from<T, Args...>)
        T& PushLayer(Args&&... args) {
            auto& layer = m_LayerStack.emplace_back(
                std::make_unique<T>(std::forward<Args>(args)...)
            );

            layer->m_App = this;
            layer->OnAttach();
            return static_cast<T&>(*layer);
        }

        Window& GetWindow() noexcept { return *m_Window; }

    private:
        void OnEvent(Event& e);

    private:
        std::unique_ptr<Window> m_Window;

        // TODO: Move layer stack to it's own dedicated class
        std::vector<std::unique_ptr<Layer>> m_LayerStack;

        double m_LastFrameTime = 0.0;
    };

}
