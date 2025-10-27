

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once

#include <cstddef>


namespace Mct::WorldConst {

    // =====================================
    // Subchunk and Chunk Dimensions
    // =====================================

    inline constexpr size_t SubchunkSizeX = 16;
    inline constexpr size_t SubchunkSizeY = 16;
    inline constexpr size_t SubchunkSizeZ = 16;


    // Chunks are divided vertically into multiple Subchunks.
    inline constexpr size_t SubchunkCount = 24;


    inline constexpr size_t ChunkSizeX = SubchunkSizeX;
    inline constexpr size_t ChunkSizeY = SubchunkSizeY * SubchunkCount;
    inline constexpr size_t ChunkSizeZ = SubchunkSizeZ;


    inline constexpr size_t ChunkBlockCount    = ChunkSizeX    * ChunkSizeY    * ChunkSizeZ;
    inline constexpr size_t SubchunkBlockCount = SubchunkSizeX * SubchunkSizeY * SubchunkSizeZ;


    // The render distance (measured in chunks).
    // TODO: Consider a better place for this.
    inline constexpr int RenderDistance = 3;

}
