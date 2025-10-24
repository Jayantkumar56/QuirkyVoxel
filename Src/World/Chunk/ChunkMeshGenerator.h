

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkSpan.h"
#include "SubchunkMesh.h"
#include "World/Block/Block.h"

#include <array>


namespace Mct {

    struct SubChunkNeighbor {
        SubchunkSpan<const Block> Main;
        SubchunkSpan<const Block> Top;    // +Y
        SubchunkSpan<const Block> Bottom; // -Y
        SubchunkSpan<const Block> North;  // -Z
        SubchunkSpan<const Block> South;  // +Z
        SubchunkSpan<const Block> East;   // +X
        SubchunkSpan<const Block> West;   // -X

        constexpr Block GetBlock(const int x, 
                                 const int y, 
                                 const int z) const noexcept
        {
            // Check Y boundaries
            if (y < 0)    return Bottom(x, 15, z);
            if (y > 15)   return Top(x, 0, z);

            // Check Z boundaries
            if (z < 0)    return North(x, y, 15);
            if (z > 15)   return South(x, y, 0);

            // Check X boundaries
            if (x < 0)    return West(15, y, z);
            if (x > 15)   return East(0, y, z);

            // Must be in the main subchunk
            return Main(x, y, z);
        }
    };

    struct ChunkNeighbor {
        ChunkSpan<const Block> Main;
        ChunkSpan<const Block> North;  // -Z
        ChunkSpan<const Block> South;  // +Z
        ChunkSpan<const Block> East;   // +X
        ChunkSpan<const Block> West;   // -X
    };

    class ChunkMeshGenerator {
    public:
        static void GenerateChunkMeshes(const ChunkNeighbor& neighbor,
                                        std::array<SubchunkCpuMeshes*, WorldConst::SubchunkCount> outMeshes);

        static void GenerateSubchunkMesh(const SubChunkNeighbor& neighbor,
                                         SubchunkCpuMeshes*      outMeshes);

    private:
        // Checks if a face should be drawn between two blocks.
        static bool ShouldDrawFace(const Block current, 
                                   const Block neighbor) noexcept;

        static void AddFaceIfVisible(size_t x, size_t y, size_t z,      // Current block coords
                                     const Block             currentBlock,
                                     const CubeNormal        faceNormal,
                                     SubchunkMesh&           targetMesh,
                                     const SubChunkNeighbor& neighbor);
    };

}
