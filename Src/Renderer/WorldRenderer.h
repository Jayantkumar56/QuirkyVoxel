

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "WorldRenderer.h"
#include "World/Chunk/ChunkCoord.h"

#include <memory>


namespace Mct {

    class  World;
    class  Camera;
    class  MeshManager;
    struct ChunkMeshes;

    class WorldRenderer {
    public:
        WorldRenderer();
        ~WorldRenderer();

        // Builds new meshes, called *before* Render
        void UpdateMeshes(World& world);

        // Draws all visible chunks, called *during* the render pass
        void Render(const Camera& camera, World& world);

    private:
        std::unique_ptr<MeshManager> m_MeshManager;
    };

}
