

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


namespace Mct {

    struct SubchunkMesh;
    struct ChunkMesh;
    struct ChunkMeshInputView;
    struct SubChunkNeighbor;

    class ChunkMeshGenerator {
    public:
        static void GenerateChunkMeshes(ChunkMesh& outMesh, const ChunkMeshInputView& neighbor);
        static void GenerateSubchunkMesh(SubchunkMesh* outMesh, const SubChunkNeighbor& neighbor);
    };

}
