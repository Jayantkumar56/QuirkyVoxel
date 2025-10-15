

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldConstants.h"
#include "Common/Assert.h"

#include <array>
#include <type_traits>


namespace Mct {

    namespace Internal {

        struct ChunkLayout {
            [[nodiscard]] static constexpr size_t IdxFromSubchunkPos(const size_t subchunkX,
                                                                     const size_t subchunkY, 
                                                                     const size_t subchunkZ) noexcept 
            {
                MCT_ASSERT(
                    subchunkX < WorldConst::SubchunkSizeX &&
                    subchunkY < WorldConst::SubchunkSizeY &&
                    subchunkZ < WorldConst::SubchunkSizeZ
                );

                return (subchunkX * WorldConst::SubchunkSizeY * WorldConst::SubchunkSizeZ) + 
                       (subchunkY * WorldConst::SubchunkSizeZ) + 
                       subchunkZ;
            }

            [[nodiscard]] static constexpr size_t SubchunkOffset(const size_t subchunkIdx) noexcept {
                MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);
                return subchunkIdx * WorldConst::SubchunkBlockCount;
            }

            [[nodiscard]] static constexpr size_t IdxFromChunkPos(const size_t chunkX, 
                                                                  const size_t chunkY, 
                                                                  const size_t chunkZ) noexcept 
            {
                MCT_ASSERT(
                    chunkX < WorldConst::ChunkSizeX &&
                    chunkY < WorldConst::ChunkSizeY &&
                    chunkZ < WorldConst::ChunkSizeZ
                );

                const size_t scY = chunkY / WorldConst::SubchunkSizeY;
                const size_t ly  = chunkY % WorldConst::SubchunkSizeY;
                return SubchunkOffset(scY) + IdxFromSubchunkPos(chunkX, ly, chunkZ);
            }
        };

    }

    template<typename T>
    class SubchunkSpan {
    public:
        constexpr SubchunkSpan(T* data) noexcept : m_Data(data) {}

        [[nodiscard]] constexpr T& operator()(const size_t x, 
                                              const size_t y, 
                                              const size_t z) noexcept 
        {
            MCT_ASSERT(m_Data != nullptr);
            return m_Data[Internal::ChunkLayout::IdxFromSubchunkPos(x, y, z)];
        }

        [[nodiscard]] constexpr const T& operator()(const size_t x,
                                                    const size_t y, 
                                                    const size_t z) const noexcept 
        {
            MCT_ASSERT(m_Data != nullptr);
            return m_Data[Internal::ChunkLayout::IdxFromSubchunkPos(x, y, z)];
        }

        constexpr operator SubchunkSpan<const T>() const noexcept
            requires (!std::is_const_v<T>)
        {
            return SubchunkSpan<const T>(m_Data);
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept { return m_Data != nullptr; }

        [[nodiscard]] constexpr size_t SizeX() const noexcept { return WorldConst::SubchunkSizeX; }
        [[nodiscard]] constexpr size_t SizeY() const noexcept { return WorldConst::SubchunkSizeY; }
        [[nodiscard]] constexpr size_t SizeZ() const noexcept { return WorldConst::SubchunkSizeZ; }

    private:
        T* m_Data;
    };

    template<typename T>
    class ChunkSpan {
    public:
        constexpr ChunkSpan(T* data) noexcept : m_Data(data) {}

        [[nodiscard]] constexpr T& operator()(const size_t x, 
                                              const size_t y, 
                                              const size_t z) noexcept 
        {
            MCT_ASSERT(m_Data != nullptr);
            return m_Data[Internal::ChunkLayout::IdxFromChunkPos(x, y, z)];
        }

        [[nodiscard]] constexpr const T& operator()(const size_t x, 
                                                    const size_t y, 
                                                    const size_t z) const noexcept 
        {
            MCT_ASSERT(m_Data != nullptr);
            return m_Data[Internal::ChunkLayout::IdxFromChunkPos(x, y, z)];
        }

        constexpr operator ChunkSpan<const T>() const noexcept
            requires (!std::is_const_v<T>)
        {
            return ChunkSpan<const T>(m_Data);
        }

        [[nodiscard]] constexpr explicit operator bool() const noexcept { return m_Data != nullptr; }

        [[nodiscard]] constexpr size_t SizeX() const noexcept { return WorldConst::ChunkSizeX; }
        [[nodiscard]] constexpr size_t SizeY() const noexcept { return WorldConst::ChunkSizeY; }
        [[nodiscard]] constexpr size_t SizeZ() const noexcept { return WorldConst::ChunkSizeZ; }

    private:
        T* m_Data;
    };

    class BlockStorage {
    public:
        constexpr BlockStorage() noexcept = default;

        [[nodiscard]] constexpr ChunkSpan<Block>       View()       noexcept { return { m_Data.data() }; }
        [[nodiscard]] constexpr ChunkSpan<const Block> View() const noexcept { return { m_Data.data() }; }

        [[nodiscard]] constexpr SubchunkSpan<Block> SubchunkView(const size_t subchunkIdx) noexcept {
            return { m_Data.data() + Internal::ChunkLayout::SubchunkOffset(subchunkIdx) };
        }

        [[nodiscard]] constexpr SubchunkSpan<const Block> SubchunkView(const size_t subchunkIdx) const noexcept {
            return { m_Data.data() + Internal::ChunkLayout::SubchunkOffset(subchunkIdx) };
        }

    private:
        std::array<Block, WorldConst::ChunkBlockCount> m_Data;
    };

}
