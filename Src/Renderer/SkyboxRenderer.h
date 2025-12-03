

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <memory>


namespace Mct {

    class Sky;
    class Camera;
    class Shader;
    class VertexArray;
    class VertexBuffer;

    class SkyboxRenderer {
    public:
        SkyboxRenderer();
        ~SkyboxRenderer();

        void Render(const Camera& camera, const Sky& sky);

    private:
        std::unique_ptr<Shader>       m_SkyShader;
        std::unique_ptr<VertexArray>  m_VAO;
        std::shared_ptr<VertexBuffer> m_VBO;

        int32_t m_DayGradientSlot;
        int32_t m_SunRiseGradientSlot;
        int32_t m_NightGradientSlot;
        int32_t m_MidnightGradientSlot;
        int32_t m_MoonTextureSlot;
    };

}