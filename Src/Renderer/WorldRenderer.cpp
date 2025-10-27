

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "WorldRenderer.h"
#include "World/World.h"
#include "MeshManager.h"
#include "World/Chunk/ChunkMeshGenerator.h"


namespace Mct {

    WorldRenderer::WorldRenderer() {

    }

    WorldRenderer::~WorldRenderer() = default;

    void WorldRenderer::Render(const Camera& camera, World& world) {
        UpdateMeshes(world);

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
                auto  subchunks = chunk->GetSubchunksForWrite();

                // Uploads mesh of each subchunks and asign the generated GpuMesh to the respective subchunk.
                for (size_t i = 0; i < subchunks.size(); ++i) {
                    auto solidMeshOpt = m_MeshManager->UploadMesh(cpuMeshes.SubchunkMeshes[i].SolidMesh);
                    auto waterMeshOpt = m_MeshManager->UploadMesh(cpuMeshes.SubchunkMeshes[i].WaterMesh);

                    if (solidMeshOpt) subchunks[i].SetSolidMesh(*solidMeshOpt);
                    if (waterMeshOpt) subchunks[i].SetWaterMesh(*waterMeshOpt);
                }
            }

            it = remeshQueue.erase(it);
        }
    }

}
