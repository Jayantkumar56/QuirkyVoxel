

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "ChunkRenderManager.h"
#include "World/World.h"
#include "MeshManager.h"
#include "Camera/Camera.h"


namespace Mct {

	void ChunkRenderManager::Update(World& world, const Camera& camera, MeshManager& meshManager) {
        m_SolidDrawCommands.clear();
        m_SubchunkPositions.clear();

        const int renderDist = WorldConst::RenderDistance;
        glm::vec3 cameraPos  = camera.GetPosition();

        ChunkCoord cameraCoord = ChunkCoord::FromWorldXZ((int)cameraPos.x, (int)cameraPos.z);
        size_t vertexStride    = meshManager.GetVBOLayout().GetStride();

        for (auto& [coord, chunk] : world.GetChunkManager().GetChunks()) {
            ProcessChunk(coord, chunk, cameraCoord, renderDist, vertexStride, meshManager);
        }
    }

    void ChunkRenderManager::ProcessChunk(const ChunkCoord& coord, std::shared_ptr<Chunk>& chunk, 
                                          const ChunkCoord& cameraCoord, int renderDist, 
                                          size_t vertexStride, MeshManager& meshManager) 
    {
        // Distance Check (Culling)
        int distX = std::abs(coord.X - cameraCoord.X);
        int distZ = std::abs(coord.Z - cameraCoord.Z);

        if (distX > renderDist || distZ > renderDist)
            return;

        // Upload if Dirty
        if (chunk->HaveDirtyMesh()) {
            UploadChunkMesh(chunk, meshManager);
        }

        // Build Commands
        // Loop through all subchunks in this chunk
        for (const auto& subchunk : chunk->GetSubchunks()) {
            glm::vec3 subchunkPos = subchunk.GetPosition();

            // Render solid meshes
            if (subchunk.HasSolidMesh()) {
                const GpuMeshHandle& mesh = subchunk.GetSolidMesh();

                MCT_ASSERT(mesh.IboHandle, "Subchunk solid mesh must have an ibo");

                const auto& ibo = *mesh.IboHandle;
                const auto& vbo = mesh.VboHandle;

                // MDI requires ELEMENT offsets, not BYTE offsets.
                // We must convert them.
                uint32_t indexSize = static_cast<uint32_t>(sizeof(uint32_t));

                m_SubchunkPositions.push_back(glm::vec4(subchunkPos.x, subchunkPos.y, subchunkPos.z, 1.0));

                m_SolidDrawCommands.emplace_back(
                    (uint32_t)(ibo.GetSize() / indexSize),        // count
                    1,                                            // instanceCount
                    (uint32_t)(ibo.GetOffset() / indexSize),      // firstIndex
                    (uint32_t)(vbo.GetOffset() / vertexStride),   // baseVertex
                    0                                             // baseInstance
                );
            }
        }
    }

    void ChunkRenderManager::UploadChunkMesh(std::shared_ptr<Chunk>& chunk, MeshManager& meshManager) {
        auto cpuMeshes = chunk->GetMeshForUpload();

        std::unique_ptr<ChunkGpuMesh> chunkGpuMesh = std::make_unique<ChunkGpuMesh>();

        // Uploads mesh of each subchunks and asign the generated GpuMeshHandle to the respective subchunk.
        for (size_t i = 0; i < cpuMeshes->SubchunkMesh.size(); ++i) {
            auto& solidMeshOpt = chunkGpuMesh->SubchunkMeshes[i].SolidMesh;
            auto& waterMeshOpt = chunkGpuMesh->SubchunkMeshes[i].WaterMesh;

            solidMeshOpt = meshManager.UploadMesh(cpuMeshes->SubchunkMesh[i].SolidMesh);
            waterMeshOpt = meshManager.UploadMesh(cpuMeshes->SubchunkMesh[i].WaterMesh);
        }

        chunk->SetGpuMesh(std::move(chunkGpuMesh));
    }

}
