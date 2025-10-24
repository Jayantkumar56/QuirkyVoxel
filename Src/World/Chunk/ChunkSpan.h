

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldConstants.h"
#include "Common/Assert.h"

#include <type_traits>


namespace Mct {

    namespace Internal {

        struct ChunkLayout {
            static constexpr size_t StrideZ = 1;
            static constexpr size_t StrideY = WorldConst::SubchunkSizeZ;                             // = SizeZ
            static constexpr size_t StrideX = WorldConst::SubchunkSizeY * WorldConst::SubchunkSizeZ; // = SizeY * SizeZ

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

            // Walk every (x,y,z) in layout order and invoke f(x,y,z, idx)
            // f must be callable with (size_t x, size_t y, size_t z, size_t idx)
            template<class Fun>
            static constexpr void ForEachIndexInSubchunk(Fun&& f)
                noexcept(std::is_nothrow_invocable_v<Fun&&, size_t, size_t, size_t, size_t>)
            {
                for (size_t x = 0; x < WorldConst::SubchunkSizeX; ++x) {
                    const size_t baseX = x * StrideX;
                    for (size_t y = 0; y < WorldConst::SubchunkSizeY; ++y) {
                        size_t idx = baseX + y * StrideY;
                        for (size_t z = 0; z < WorldConst::SubchunkSizeZ; ++z, ++idx) {
                            std::invoke(std::forward<Fun>(f), x, y, z, idx);
                        }
                    }
                }
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

        template<typename Fun>
        constexpr void ForEachXYZ(Fun&& fun) 
            noexcept(std::is_nothrow_invocable_v<Fun&&, size_t, size_t, size_t, size_t>)
            requires (!std::is_const_v<T>)
        {
            MCT_ASSERT(m_Data != nullptr);

            Internal::ChunkLayout::ForEachIndexInSubchunk([this, &fun](size_t x, size_t y, size_t z, size_t idx) 
                noexcept(std::is_nothrow_invocable_v<Fun&&, size_t, size_t, size_t, T&>)
            {
                std::invoke(std::forward<Fun>(fun), x, y, z, m_Data[idx]);
            });
        }

        template<typename Fun>
        constexpr void ForEachConstXYZ(Fun&& fun) const
            noexcept(std::is_nothrow_invocable_v<Fun&&, size_t, size_t, size_t, const T&>)
        {
            MCT_ASSERT(m_Data != nullptr);

            Internal::ChunkLayout::ForEachIndexInSubchunk([this, &fun](size_t x, size_t y, size_t z, size_t idx)
                noexcept(std::is_nothrow_invocable_v<Fun&&, size_t, size_t, size_t, const T&>)
            {
                std::invoke(std::forward<Fun>(fun), x, y, z, m_Data[idx]);
            });
        }

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

        [[nodiscard]] constexpr SubchunkSpan<T> GetSubchunkForWrite(const size_t subchunkIdx) noexcept
            requires (!std::is_const_v<T>)
        {
            MCT_ASSERT(m_Data != nullptr);
            MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);

            return { &m_Data[Internal::ChunkLayout::SubchunkOffset(subchunkIdx)] };
        }

        [[nodiscard]] constexpr SubchunkSpan<const T> GetSubchunkForRead(const size_t subchunkIdx) const noexcept {
            MCT_ASSERT(m_Data != nullptr);
            MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);

            return { &m_Data[Internal::ChunkLayout::SubchunkOffset(subchunkIdx)] };
        }

    private:
        T* m_Data;
    };

}
