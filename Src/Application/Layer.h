

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


namespace Mct {

    class Event;

    class Application;

    class Layer {
        friend class Application;

    public:
        virtual ~Layer() = default;

        // Called once when the layer is attached to the application.
        virtual void OnAttach() {}

        // Called once when the layer is detached from the application.
        virtual void OnDetach() {}

        // Called every frame.
        virtual void OnEvent(Event& e) {}

        // Called every frame.
        virtual void OnUpdate(float deltaTime) {}

        Application* GetApp() noexcept { return m_App; }

    private:
        Application* m_App = nullptr;
    };

}
