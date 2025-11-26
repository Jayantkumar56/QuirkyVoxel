

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "WorldRenderer.h"
#include "World/World.h"
#include "MeshManager.h"
#include "Primitives/TextureArray.h"
#include "Camera/Camera.h"
#include "Renderer/Primitives/Shader.h"
#include "Utils/FileUtils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Mct {

    constexpr size_t c_MaxDrawCommands = 5000;

    WorldRenderer::WorldRenderer() :
            m_ChunkOffsetSSBO ( c_MaxDrawCommands * sizeof(glm::vec4) ),
            m_MdiBuffer       ( c_MaxDrawCommands                     )
    {
        m_MeshManager = std::make_unique<MeshManager>(
            PackedTerrainMesh::GetBufferLayout(),
            134217728, // 128 MiB
            134217728, // 128 MiB
            16384,     // 16 KiB
            16384      // 16 KiB
        );

        // Sets up textures.
        {
            const std::vector<std::string>& blockTexturesPaths = BlockDataManager::GetTextureFilePaths();
            m_BlockTextureArray = std::make_unique<TextureArray>(blockTexturesPaths);
        }

        // Sets up Terrain Shader.
        {
            std::string vertexSrc   = Utils::ReadFileToString("Assets/Shaders/Terrain.vert");
            std::string fragmentSrc = Utils::ReadFileToString("Assets/Shaders/Terrain.frag");

            m_TerrainShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

            // Sets value of permanent uniforms.
            {
                m_TerrainShader->Bind();

                int32_t blockTexturesTexUnit = 0;
                m_TerrainShader->UploadUniform("u_BlockTextures", &blockTexturesTexUnit, 1);
                m_TerrainShader->Unbind();
            }
        }
    }

    WorldRenderer::~WorldRenderer() = default;

    void WorldRenderer::Render(const Camera& camera, World& world) {
        m_MeshManager->Update();
        m_ChunkRendererManager.Update(world, camera, *m_MeshManager);

        // Bind Shader and perform draw call
        {
            const auto& commands = m_ChunkRendererManager.GetSolidCommands();

            if (commands.empty()) {
                m_TerrainShader->Unbind();
                return; // Nothing to draw
            }

            m_TerrainShader->Bind();

            m_BlockTextureArray->Bind();

            m_TerrainShader->UploadUniform("u_ViewProjection", camera.GetViewProjection());
            m_TerrainShader->UploadUniform("u_CameraPos", camera.GetPosition());

            m_MeshManager->GetCommonVAO().Bind();

            m_MdiBuffer.Bind();
            m_MdiBuffer.UploadData(commands.data(), commands.size());

            const auto& offsets = m_ChunkRendererManager.GetSubchunkOffsets();

            m_ChunkOffsetSSBO.Upload(offsets.data(), offsets.size() * sizeof(glm::vec4), 0);
            m_ChunkOffsetSSBO.Bind(1);

            glEnable(GL_DEPTH_TEST);
            glMultiDrawElementsIndirect(
                GL_TRIANGLES,                           // mode
                GL_UNSIGNED_INT,                        // type (must match your EBO type)
                (void*)0,                               // indirect (offset into MDI buffer)
                static_cast<GLsizei>(commands.size()),  // drawcount
                sizeof(DrawElementsIndirectCommand)     // stride
            );

            m_MeshManager->GetCommonVAO().Unbind();
            m_MdiBuffer.Unbind();

            m_TerrainShader->Unbind();
        }

        glm::vec3 sunDir{ 0.0f, 0.5f, -1.0f };

        m_SkyboxRenderer.Render(camera, sunDir);
    }

}
