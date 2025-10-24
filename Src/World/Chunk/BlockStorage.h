

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkSpan.h"
#include "World/WorldConstants.h"
#include "Common/Assert.h"

#include <array>


namespace Mct {

    class BlockStorage {
    public:
        constexpr BlockStorage() noexcept = default;

        [[nodiscard]] constexpr ChunkSpan<Block>       View()       noexcept { return { m_Data.data() }; }
        [[nodiscard]] constexpr ChunkSpan<const Block> View() const noexcept { return { m_Data.data() }; }

        [[nodiscard]] constexpr SubchunkSpan<Block> GetSubchunkForWrite(const size_t subchunkIdx) noexcept {
            MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);
            return { &m_Data[Internal::ChunkLayout::SubchunkOffset(subchunkIdx)] };
        }

        [[nodiscard]] constexpr SubchunkSpan<const Block> GetSubchunkForRead(const size_t subchunkIdx) const noexcept {
            MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);
            return { &m_Data[Internal::ChunkLayout::SubchunkOffset(subchunkIdx)] };
        }

    private:
        std::array<Block, WorldConst::ChunkBlockCount> m_Data;
    };

}
