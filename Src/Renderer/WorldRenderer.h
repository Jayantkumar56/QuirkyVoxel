

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Primitives/IndirectBuffer.h"
#include "Primitives/ShaderStorageBuffer.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>


namespace Mct {

    class TextureArray;
    class Shader;
    class World;
    class Camera;
    class MeshManager;

    struct DrawElementsIndirectCommand {
        uint32_t count;         // Number of indices (IndexCount)
        uint32_t instanceCount; // Always 1 for us
        uint32_t firstIndex;    // Element offset (IndexOffset)
        uint32_t baseVertex;    // Element offset (VertexOffset)
        uint32_t baseInstance;  // Always 0 for us
    };

    class WorldRenderer {
    public:
        WorldRenderer();
        ~WorldRenderer();

        // Draws all visible chunks, called during the render pass.
        void Render(const Camera& camera, World& world);

    private:
        // Builds new meshes, called before Render
        void UpdateMeshes(World& world);

        void BuildRenderCommands(World& world, const Camera& camera);

    private:
        ShaderStorageBuffer    m_ChunkOffsetSSBO;
        std::vector<glm::vec4> m_SubchunkPositions;

        IndirectBuffer<DrawElementsIndirectCommand> m_MdiBuffer;
        std::vector<DrawElementsIndirectCommand>    m_DrawCommands;

        std::unique_ptr<Shader>       m_TerrainShader;
        std::unique_ptr<TextureArray> m_BlockTextureArray;
        std::unique_ptr<MeshManager>  m_MeshManager;
    };

}
