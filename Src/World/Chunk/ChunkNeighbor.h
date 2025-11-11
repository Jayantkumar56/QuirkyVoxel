

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkSpan.h"
#include "Chunk.h"

#include <memory>


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

    struct ChunkMeshInput {
        friend struct ChunkMeshInputView;

    public:
        // These spans are guaranteed to be valid for the
        // lifetime of this struct object.

        ChunkMeshInput(std::shared_ptr<Chunk>       main,
                       std::shared_ptr<const Chunk> north,
                       std::shared_ptr<const Chunk> south,
                       std::shared_ptr<const Chunk> east,
                       std::shared_ptr<const Chunk> west) :
                m_MainPtr  ( std::move(main)    ),
                m_NorthPtr ( std::move(north)   ),
                m_SouthPtr ( std::move(south)   ),
                m_EastPtr  ( std::move(east)    ),
                m_WestPtr  ( std::move(west)    )
        {}

        bool IsValid() const noexcept {
            return m_MainPtr && m_NorthPtr && m_SouthPtr && m_EastPtr && m_WestPtr;
        }

        std::shared_ptr<Chunk>& GetMainChunk() noexcept { return m_MainPtr; }

    private:
        // These are kept alive until this struct is destroyed,
        // which guarantees the spans above are always valid.

        std::shared_ptr<Chunk>       m_MainPtr;
        std::shared_ptr<const Chunk> m_NorthPtr;
        std::shared_ptr<const Chunk> m_SouthPtr;
        std::shared_ptr<const Chunk> m_EastPtr;
        std::shared_ptr<const Chunk> m_WestPtr;
    };

    struct ChunkMeshInputView {
        const ChunkSpan<const Block> Main;
        const ChunkSpan<const Block> North;
        const ChunkSpan<const Block> South;
        const ChunkSpan<const Block> East;
        const ChunkSpan<const Block> West;

        ChunkMeshInputView(ChunkMeshInput& meshInput) : 
                Main       ( meshInput.m_MainPtr->GetBlocks()  ),
                North      ( meshInput.m_NorthPtr->GetBlocks() ),
                South      ( meshInput.m_SouthPtr->GetBlocks() ),
                East       ( meshInput.m_EastPtr->GetBlocks()  ),
                West       ( meshInput.m_WestPtr->GetBlocks()  )
        {}
    };

}
