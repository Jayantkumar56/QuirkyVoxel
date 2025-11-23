

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>

#include <vector>
#include <memory>


namespace Mct {

    class World;
    class Camera;
    class MeshManager;
    class Chunk;

    struct ChunkCoord;

    struct DrawElementsIndirectCommand {
        uint32_t count;         // Number of indices (IndexCount)
        uint32_t instanceCount; // Always 1 for us
        uint32_t firstIndex;    // Element offset (IndexOffset)
        uint32_t baseVertex;    // Element offset (VertexOffset)
        uint32_t baseInstance;  // Always 0 for us
    };

    class ChunkRenderManager {
    public:
        void Update(World& world, const Camera& camera, MeshManager& meshManager);

        const std::vector<DrawElementsIndirectCommand>& GetSolidCommands() const { return m_SolidDrawCommands; }
        const std::vector<glm::vec4>& GetSubchunkOffsets() const { return m_SubchunkPositions; }

    private:
        void ProcessChunk(const ChunkCoord& coord, std::shared_ptr<Chunk>& chunk,
                          const ChunkCoord& cameraCoord, int renderDist,
                          size_t vertexStride, MeshManager& meshManager);

        void UploadChunkMesh(std::shared_ptr<Chunk>& chunk, MeshManager& meshManager);

    private:
        std::vector<glm::vec4>                   m_SubchunkPositions;
        std::vector<DrawElementsIndirectCommand> m_SolidDrawCommands;
    };

}
