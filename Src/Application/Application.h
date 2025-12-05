

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "LayerManager.h"


namespace Mct {

    class Window;

    class Application {
    public:
        Application() noexcept;
        ~Application();

        void Run();

        template<typename T, typename ...Args>
        requires (std::derived_from<T, Layer>&& std::constructible_from<T, Args...>)
        T& PushLayer(Args&& ...args) {
            m_LayerManager.PushLayer(std::forward<Args>(args)...);
        }

        Window& GetWindow() noexcept { return *m_Window; }

    private:
        std::unique_ptr<Window> m_Window;

        LayerManager m_LayerManager;

        double m_LastFrameTime = 0.0;
    };

}
