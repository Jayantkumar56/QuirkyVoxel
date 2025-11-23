

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>

#include <memory>


namespace Mct {

    class Camera;
    class Shader;
    class VertexArray;
    class VertexBuffer;

    class SkyboxRenderer {
    public:
        SkyboxRenderer();
        ~SkyboxRenderer();

        // sunDir needs to be normalized.
        void Render(const Camera& camera, const glm::vec3& sunDir);

    private:
        std::unique_ptr<Shader>       m_SkyShader;
        std::unique_ptr<VertexArray>  m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;
    };

}
