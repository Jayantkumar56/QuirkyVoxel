

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "SkyboxRenderer.h"
#include "Utils/FileUtils.h"
#include "Primitives/VertexArray.h"
#include "Primitives/Shader.h"
#include "World/Sky.h"
#include "Camera/Camera.h"
#include "Utils/Logger.h"
#include "Utils/CubeData.h"

#include <glad/glad.h>


namespace Mct {

	Mct::SkyboxRenderer::SkyboxRenderer() {
        // Shader
        {
            const std::filesystem::path vertSrcPath = "Assets/Shaders/Skybox.vert";
            const std::filesystem::path fragSrcPath = "Assets/Shaders/Skybox.frag";

            const auto vertSrc = Utils::ReadFileToString(vertSrcPath);
            const auto fragSrc = Utils::ReadFileToString(fragSrcPath);

            if (!vertSrc) {
                MCT_ERROR("Unable to read shader file at location: {}", vertSrcPath.string()); 
                return;
            }


            if (!fragSrc) {
                MCT_ERROR("Unable to read shader file at location: {}", fragSrcPath.string()); 
                return;
            }

            m_SkyShader = std::make_unique<Shader>(*vertSrc, *fragSrc);

            // Sets constant uniforms
            {
                m_DayGradientSlot      = 0;
                m_SunRiseGradientSlot  = 1;
                m_NightGradientSlot    = 2;
                m_MidnightGradientSlot = 3;
                m_MoonTextureSlot      = 4;

                m_SkyShader->UploadUniform("u_DaySkyGradient",      &m_DayGradientSlot,      1);
                m_SkyShader->UploadUniform("u_SunriseSkyGradient",  &m_SunRiseGradientSlot,  1);
                m_SkyShader->UploadUniform("u_NightSkyGradient",    &m_NightGradientSlot,    1);
                m_SkyShader->UploadUniform("u_MidnightSkyGradient", &m_MidnightGradientSlot, 1);
                m_SkyShader->UploadUniform("u_MoonTexture",         &m_MoonTextureSlot,      1);
            }
        }

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(sizeof(CUBE_POSITIONS));

        m_VBO->Upload(CUBE_POSITIONS.data(), sizeof(CUBE_POSITIONS), 0);

        m_VBO->SetLayout({ {ShaderDataType::Float3, "i_Pos"} });
        m_VAO->AddVertexBuffer(m_VBO);
	}

    SkyboxRenderer::~SkyboxRenderer() = default;

	void SkyboxRenderer::Render(const Camera& camera, const Sky& sky) {
        const glm::vec3 sunDir       = sky.GetSunDirection();
        const glm::vec3 moonDir      = sky.GetMoonDirection();
        const glm::mat4 projection   = camera.GetProjection();
        const glm::mat4 view         = camera.GetView();

        const float sunRadius  = sky.GetSunRadius();
        const float moonRadius = sky.GetMoonRadius();

        const float sunVisibility  = sky.GetSunVisibility();
        const float moonVisibility = sky.GetMoonVisibility();

        // Converting to mat3 and back to mat4 keeps rotation/scale but zeroes translation.
        // The skybox must always appear centered on the camera (distance = infinite).
        const glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
        const glm::mat4 skyViewProjection = projection * viewNoTranslation;

        const Texture2D& sunriseGradient  = sky.GetSunriseGradient();
        const Texture2D& dayGradient      = sky.GetDayGradient();
        const Texture2D& nightGradient    = sky.GetNightGradient();
        const Texture2D& midnightGradient = sky.GetMidnightGradient();

        const Texture2D& moonTexture = sky.GetMoonTexture();

        const SkyBlendWeights gradientWeights = sky.GetGradientWeights();

        // Render call
        {
            m_SkyShader->Bind();

            sunriseGradient.Bind(m_SunRiseGradientSlot);
            dayGradient.Bind(m_DayGradientSlot);
            nightGradient.Bind(m_NightGradientSlot);
            midnightGradient.Bind(m_MidnightGradientSlot);

            moonTexture.Bind(m_MoonTextureSlot);

            // Upload Uniforms
            m_SkyShader->UploadUniform("u_ViewProjection", skyViewProjection);

            m_SkyShader->UploadUniform("u_SunDir",  sunDir );
            m_SkyShader->UploadUniform("u_MoonDir", moonDir);
            m_SkyShader->UploadUniform("u_SunRadius",  &sunRadius, 1 );
            m_SkyShader->UploadUniform("u_MoonRadius", &moonRadius, 1);

            m_SkyShader->UploadUniform("u_DaySkyWeight",      &gradientWeights.Day,      1);
            m_SkyShader->UploadUniform("u_SunriseSkyWeight",  &gradientWeights.Sunrise,  1);
            m_SkyShader->UploadUniform("u_NightSkyWeight",    &gradientWeights.Night,    1);
            m_SkyShader->UploadUniform("u_MidnightSkyWeight", &gradientWeights.Midnight, 1);

            m_SkyShader->UploadUniform("u_SunVisibility",  &sunVisibility,  1);
            m_SkyShader->UploadUniform("u_MoonVisibility", &moonVisibility, 1);

            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);

            m_VAO->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            m_VAO->Unbind();

            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);

            m_SkyShader->Unbind();
        }
	}

}
