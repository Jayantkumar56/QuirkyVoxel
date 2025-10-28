

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Window.h"
#include "Layer.h"

#include <vector>
#include <type_traits>
#include <memory>


namespace Mct {

    class Application {
    public:
        Application() noexcept;
        ~Application();

        void Run();

        template<typename T, typename... Args>
        requires (std::derived_from<T, Layer>&& std::constructible_from<T, Args...>)
        T& PushLayer(Args&&... args) {
            auto& up = m_LayerStack.emplace_back(
                std::make_unique<T>(std::forward<Args>(args)...)
            );

            up->OnAttach();
            return static_cast<T&>(*up);
        }

    private:
        void InitImgui();
        void TerminateImgui();

        void BeginImgui();
        void EndImgui();

    private:
        std::optional<Window> m_Window;

        // TODO: Move layer stack to it's own dedicated class
        std::vector<std::unique_ptr<Layer>> m_LayerStack;
    };

}
