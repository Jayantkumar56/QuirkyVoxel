

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkMesh.h"


namespace Mct {

    struct ChunkMeshInput;
    struct SubChunkNeighbor;

    class ChunkMeshGenerator {
    public:
        static ChunkMesh GenerateChunkMeshes(const ChunkMeshInput& neighbor);

        static void GenerateSubchunkMesh(const SubChunkNeighbor& neighbor,
                                         SubchunkMesh*         outMeshes);
    };

}
