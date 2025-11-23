

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Primitives/IndirectBuffer.h"
#include "Primitives/ShaderStorageBuffer.h"
#include "SkyboxRenderer.h"
#include "ChunkRenderManager.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>


namespace Mct {

    class TextureArray;
    class Shader;
    class World;
    class Camera;
    class MeshManager;

    class WorldRenderer {
    public:
        WorldRenderer();
        ~WorldRenderer();

        // Draws all visible chunks, called during the render pass.
        void Render(const Camera& camera, World& world);

    private:
        ShaderStorageBuffer m_ChunkOffsetSSBO;
        IndirectBuffer<DrawElementsIndirectCommand> m_MdiBuffer;

        ChunkRenderManager m_ChunkRendererManager;
        SkyboxRenderer     m_SkyboxRenderer;

        std::unique_ptr<Shader>       m_TerrainShader;
        std::unique_ptr<TextureArray> m_BlockTextureArray;
        std::unique_ptr<MeshManager>  m_MeshManager;
    };

}
