

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "WorldRenderer.h"
#include "World/World.h"
#include "MeshManager.h"
#include "Primitives/TextureArray.h"
#include "Camera/Camera.h"
#include "Renderer/Primitives/Shader.h"
#include "World/Chunk/ChunkMeshGenerator.h"
#include "Utils/FileUtils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Mct {

    constexpr size_t c_MaxDrawCommands = 20;

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

        m_DrawCommands.reserve(c_MaxDrawCommands);

        // Sets up Terrain Shader.
        {
            std::string vertexSrc   = Utils::ReadFileToString("Assets/Shaders/Terrain.vert");
            std::string fragmentSrc = Utils::ReadFileToString("Assets/Shaders/Terrain.frag");

            m_TerrainShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

            // Sets value of permanent uniforms.
            m_TerrainShader->Bind();
            m_TerrainShader->UploadUniform("u_BlockTextures", 0);
            m_TerrainShader->Unbind();
        }
    }

    WorldRenderer::~WorldRenderer() = default;

    void WorldRenderer::Render(const Camera& camera, World& world) {
        UpdateMeshes(world);

        BuildRenderCommands(world, camera);

        // Bind Shader and perform draw call
        {
            if (m_DrawCommands.empty()) {
                m_TerrainShader->Unbind();
                return; // Nothing to draw
            }

            m_TerrainShader->Bind();

            m_BlockTextureArray->Bind();

            m_TerrainShader->UploadUniform("u_ViewProjection", camera.GetViewProjection());

            m_MeshManager->GetCommonVAO().Bind();

            m_MdiBuffer.Bind();
            m_MdiBuffer.UploadData(m_DrawCommands.data(), m_DrawCommands.size());

            m_ChunkOffsetSSBO.Upload(m_SubchunkPositions.data(), m_SubchunkPositions.size() * sizeof(glm::vec4), 0);
            m_ChunkOffsetSSBO.Bind(1);
            m_SubchunkPositions.clear();

            glEnable(GL_DEPTH_TEST);
            glMultiDrawElementsIndirect(
                GL_TRIANGLES,                       // mode
                GL_UNSIGNED_INT,                    // type (must match your EBO type)
                (void*)0,                           // indirect (offset into MDI buffer)
                m_DrawCommands.size(),              // drawcount
                sizeof(DrawElementsIndirectCommand) // stride
            );

            m_MeshManager->GetCommonVAO().Unbind();
            m_MdiBuffer.Unbind();

            m_TerrainShader->Unbind();
        }
    }

    void WorldRenderer::UpdateMeshes(World& world) {
        auto& chunkManager = world.GetChunkManager();
        auto& remeshQueue  = chunkManager.GetRemeshQueueToProcess();

        // Use an iterator to safely remove items while looping
        auto it = remeshQueue.begin();

        while (it != remeshQueue.end()) {
            ChunkCoord coord = *it;
            std::optional<ChunkMeshInput> chunkMeshInput = chunkManager.GetChunkMeshInputData(coord);

            if (chunkMeshInput) {
                auto  cpuMeshes = ChunkMeshGenerator::GenerateChunkMeshes(*chunkMeshInput);
                auto& chunk     = chunkMeshInput->GetMainChunk();

                std::unique_ptr<ChunkGpuMesh> chunkGpuMesh = std::make_unique<ChunkGpuMesh>();

                // Uploads mesh of each subchunks and asign the generated GpuMesh to the respective subchunk.
                for (size_t i = 0; i < cpuMeshes.SubchunkMesh.size(); ++i) {
                    auto solidMeshOpt = m_MeshManager->UploadMesh(cpuMeshes.SubchunkMesh[i].SolidMesh);
                    auto waterMeshOpt = m_MeshManager->UploadMesh(cpuMeshes.SubchunkMesh[i].WaterMesh);

                    chunkGpuMesh->SubchunkMeshes[i].SolidMesh = solidMeshOpt;
                    chunkGpuMesh->SubchunkMeshes[i].WaterMesh = waterMeshOpt;
                }

                chunk->SetMesh(std::move(chunkGpuMesh));
            }

            it = remeshQueue.erase(it);
        }
    }

    void WorldRenderer::BuildRenderCommands(World& world, const Camera& camera) {
        m_DrawCommands.clear();

        const int renderDist = WorldConst::RenderDistance;

        glm::vec3 cameraPos    = camera.GetPosition();
        ChunkCoord cameraCoord = ChunkCoord::FromWorldXZ(static_cast<int>(cameraPos.x), static_cast<int>(cameraPos.z));

        // Get info needed to convert byte offsets to element offsets
        const size_t vertexStride = m_MeshManager->GetVBOLayout().GetStride();

        // Loop through all chunks
        for (auto& [coord, chunk] : world.GetChunkManager().GetChunks()) {
            const int distFromCameraX = std::abs(coord.X - cameraCoord.X);
            const int distFromCameraZ = std::abs(coord.Z - cameraCoord.Z);

            if (distFromCameraX > renderDist || distFromCameraZ > renderDist)
                continue;

            // Loop through all subchunks in this chunk
            for (const auto& subchunk : chunk->GetSubchunks()) {
                glm::vec3 subchunkPos = subchunk.GetPosition();

                // Render solid meshes
                if (subchunk.HasSolidMesh()) {
                    const GpuMesh& mesh = subchunk.GetSolidMesh();

                    // We can only draw if it has an index buffer
                    if (mesh.IboHandle) {
                        const auto& ibo = *mesh.IboHandle;
                        const auto& vbo = mesh.VboHandle;

                        // MDI requires ELEMENT offsets, not BYTE offsets.
                        // We must convert them.
                        uint32_t indexSize = static_cast<uint32_t>(sizeof(uint32_t));

                        m_SubchunkPositions.push_back(glm::vec4(subchunkPos.x, subchunkPos.y, subchunkPos.z, 1.0));

                        m_DrawCommands.emplace_back(
                            (uint32_t)(ibo.GetSize() / indexSize),     // count
                            1,                                         // instanceCount
                            (uint32_t)(ibo.GetOffset() / indexSize),   // firstIndex
                            (uint32_t)(vbo.GetOffset() / vertexStride),// baseVertex
                            0                                          // baseInstance
                        );
                    }
                }
                // (You would do a separate pass for water/transparent meshes)
            }
        }
    }

}
