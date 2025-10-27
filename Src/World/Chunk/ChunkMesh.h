

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "PackedTerrainMesh.h"
#include "World/WorldConstants.h"

#include <array>


namespace Mct {

    struct SubchunkMeshes {
        PackedTerrainMesh SolidMesh;
        PackedTerrainMesh WaterMesh;
        // TODO: Have mesh for transparent blocks.
    };

    struct ChunkMeshes {
        std::array<SubchunkMeshes, WorldConst::SubchunkCount> SubchunkMeshes;
    };

}
