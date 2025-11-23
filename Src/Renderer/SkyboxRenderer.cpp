

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "SkyboxRenderer.h"
#include "Utils/FileUtils.h"
#include "Primitives/VertexArray.h"
#include "Primitives/Shader.h"
#include "Camera/Camera.h"

#include <glad/glad.h>

#include <array>


namespace Mct {

    // Simple Unit Cube (Positions Only)
    // 36 Vertices (6 faces * 2 triangles * 3 vertices)
    static constexpr std::array s_SkyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	Mct::SkyboxRenderer::SkyboxRenderer() {
        std::string vertSrc = Utils::ReadFileToString("Assets/Shaders/Skybox.vert");
        std::string fragSrc = Utils::ReadFileToString("Assets/Shaders/Skybox.frag");
        m_SkyShader = std::make_unique<Shader>(vertSrc, fragSrc);

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(sizeof(s_SkyboxVertices));

        m_VBO->Upload(s_SkyboxVertices.data(), sizeof(s_SkyboxVertices), 0);

        m_VBO->SetLayout({ {ShaderDataType::Float3, "i_Pos"} });
        m_VAO->AddVertexBuffer(m_VBO);
	}

    SkyboxRenderer::~SkyboxRenderer() = default;

	void SkyboxRenderer::Render(const Camera& camera, const glm::vec3& sunDir) {
        glm::mat4 projection = camera.GetProjection();
        glm::mat4 view = camera.GetView();

        // Converting to mat3 and back to mat4 keeps rotation/scale but zeroes translation.
        // The skybox must always appear centered on the camera (distance = infinite).
        glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
        glm::mat4 skyViewProjection = projection * viewNoTranslation;

        glDepthFunc(GL_LEQUAL);

        glDepthMask(GL_FALSE);

        m_SkyShader->Bind();

        // Upload Uniforms
        m_SkyShader->UploadUniform("u_ViewProjection", skyViewProjection);
        m_SkyShader->UploadUniform("u_SunPos", sunDir);

        m_VAO->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_VAO->Unbind();

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        m_SkyShader->Unbind();
	}

}
