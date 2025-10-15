

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


namespace Mct {

    class Layer {
    public:
        virtual ~Layer() = default;

        // Called once when the layer is attached to the application.
        virtual void OnAttach() {}

        // Called once when the layer is detached from the application.
        virtual void OnDetach() {}

        // Called every frame.
        virtual void OnUpdate() {}
    };

}
