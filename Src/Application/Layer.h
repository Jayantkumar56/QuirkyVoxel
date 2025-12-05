

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/IdGenerator.h"


namespace Mct {

    class Event;

    class Application;

    class Layer {
        friend class LayerManager;

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

        constexpr bool         IsDisabled()    const noexcept { return m_Disabled;   }
        constexpr size_t       GetTypeId()     const noexcept { return m_TypeId;     }
        constexpr size_t       GetInstanceId() const noexcept { return m_InstanceId; }
        constexpr Application* GetApp()              noexcept { return m_App;        }

    private:
        bool         m_Disabled   = true;
        size_t       m_TypeId     = InvalidTypeId;
        size_t       m_InstanceId = InvalidId<size_t>;
        Application* m_App        = nullptr;
    };

}
