

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Renderer/Mesh/GpuMeshHandle.h"
#include "World/WorldConstants.h"

#include <optional>


namespace Mct {

    struct SubchunkGpuMesh {
        std::optional<GpuMeshHandle> SolidMesh;
        std::optional<GpuMeshHandle> WaterMesh;
        // TODO: Have mesh for transparent blocks.
    };

    struct ChunkGpuMesh {
        std::array<SubchunkGpuMesh, WorldConst::SubchunkCount> SubchunkMeshes;
    };

}
