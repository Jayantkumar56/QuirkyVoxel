

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>


namespace Mct {

    class Camera {
    public:
        virtual ~Camera() = default;

        virtual glm::mat4 GetViewProjection() const noexcept = 0;
        virtual glm::mat4 GetView()           const noexcept = 0;
        virtual glm::mat4 GetProjection()     const noexcept = 0;
        virtual glm::vec3 GetPosition()       const noexcept = 0;
    };

}
