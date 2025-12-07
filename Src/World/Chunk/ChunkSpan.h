

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldConstants.h"
#include "Utils/Assert.h"

#include <QkTraits/FunctionTraits.h>


namespace Mct {

    namespace Internal {

        struct ChunkLayout {
            static constexpr size_t StrideZ = 1;
            static constexpr size_t StrideY = WorldConst::SubchunkSizeZ;                             // = SizeZ
            static constexpr size_t StrideX = WorldConst::SubchunkSizeY * WorldConst::SubchunkSizeZ; // = SizeY * SizeZ

            [[nodiscard]] static constexpr size_t IdxFromSubchunkPos(size_t subchunkX,
                                                                     size_t subchunkY,
                                                                     size_t subchunkZ) noexcept
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

            [[nodiscard]] static constexpr size_t SubchunkOffset(size_t subchunkIdx) noexcept {
                MCT_ASSERT(subchunkIdx < WorldConst::SubchunkCount);
                return subchunkIdx * WorldConst::SubchunkBlockCount;
            }

            [[nodiscard]] static constexpr size_t IdxFromChunkPos(size_t chunkX,
                                                                  size_t chunkY,
                                                                  size_t chunkZ) noexcept
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
            template<typename Fun>
            static constexpr void ForEachIndexInSubchunk(Fun&& fun)
                    noexcept(std::is_nothrow_invocable_v<Fun,
                        typename QkT::Function<Fun>::ArgsT::template Get<0>, // x
                        typename QkT::Function<Fun>::ArgsT::template Get<1>, // y
                        typename QkT::Function<Fun>::ArgsT::template Get<2>, // z
                        typename QkT::Function<Fun>::ArgsT::template Get<3>  // idx
                    >)
            {
                using functorT = QkT::Function<Fun>;

                static_assert(std::is_same_v<typename functorT::ReturnT, void>);
                static_assert(functorT::ArgsT::Size == 4);
            
                using typeX     = typename functorT::ArgsT::template Get<0>;
                using typeY     = typename functorT::ArgsT::template Get<1>;
                using typeZ     = typename functorT::ArgsT::template Get<2>;
                using blockIdxT = typename functorT::ArgsT::template Get<3>;

                static_assert(
                    std::is_integral_v<typeX> && 
                    std::is_integral_v<typeY> && 
                    std::is_integral_v<typeZ> &&
                    std::is_integral_v<blockIdxT>
                );

                constexpr typeX subchunkSizeX = static_cast<typeX>(WorldConst::SubchunkSizeX);
                constexpr typeY subchunkSizeY = static_cast<typeY>(WorldConst::SubchunkSizeY);
                constexpr typeZ subchunkSizeZ = static_cast<typeZ>(WorldConst::SubchunkSizeZ);

                for (typeX x = 0; x < subchunkSizeX; ++x) {
                    const blockIdxT baseX = static_cast<blockIdxT>(x) * static_cast<blockIdxT>(StrideX);

                    for (typeY y = 0; y < subchunkSizeY; ++y) {
                        blockIdxT blockIdx = baseX + static_cast<blockIdxT>(y) * static_cast<blockIdxT>(StrideY);

                        for (typeZ z = 0; z < subchunkSizeZ; ++z, ++blockIdx) {
                            std::invoke(fun, x, y, z, blockIdx);
                        }
                    }
                }
            }

            template<class Fun>
            static constexpr void ForEachIndexInChunk(Fun&& f)
                    noexcept(std::is_nothrow_invocable_v<Fun,
                        typename QkT::Function<Fun>::ArgsT::template Get<0>, // x
                        typename QkT::Function<Fun>::ArgsT::template Get<1>, // y
                        typename QkT::Function<Fun>::ArgsT::template Get<2>, // z
                        typename QkT::Function<Fun>::ArgsT::template Get<3>  // idx
                    >)
            {
                using functorT = QkT::Function<Fun>;

                static_assert(std::is_same_v<typename functorT::ReturnT, void>);
                static_assert(functorT::ArgsT::Size == 4);
            
                using typeX     = typename functorT::ArgsT::template Get<0>;
                using typeY     = typename functorT::ArgsT::template Get<1>;
                using typeZ     = typename functorT::ArgsT::template Get<2>;
                using blockIdxT = typename functorT::ArgsT::template Get<3>;

                static_assert(
                    std::is_integral_v<typeX> && 
                    std::is_integral_v<typeY> && 
                    std::is_integral_v<typeZ> &&
                    std::is_integral_v<blockIdxT>
                );

                for (size_t scY = 0; scY < WorldConst::SubchunkCount; ++scY) {
                    // Base memory offset for this subchunk
                    const blockIdxT subchunkBaseOffset = SubchunkOffset(scY);

                    // Base world-Y coordinate for this subchunk
                    const blockIdxT subchunkBaseY = static_cast<blockIdxT>(scY) * WorldConst::SubchunkSizeY;

                    ForEachIndexInSubchunk(
                        [&](typeX x, typeY y, typeZ z, blockIdxT localIdx)
                            noexcept(std::is_nothrow_invocable_v<Fun&&, typeX, typeY, typeZ, blockIdxT>)
                        {
                            // Calculate the chunk coordinates
                            const blockIdxT chunkX = static_cast<blockIdxT>(x);
                            const blockIdxT chunkY = subchunkBaseY + static_cast<blockIdxT>(y);
                            const blockIdxT chunkZ = static_cast<blockIdxT>(z);

                            // Calculate the final chunk index
                            const blockIdxT chunkIdx = subchunkBaseOffset + localIdx;

                            std::invoke(std::forward<Fun>(f), chunkX, chunkY, chunkZ, chunkIdx);
                        }
                    );
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
                noexcept(std::is_nothrow_invocable_v<Fun,
                    typename QkT::Function<Fun>::ArgsT::template Get<0>,
                    typename QkT::Function<Fun>::ArgsT::template Get<1>,
                    typename QkT::Function<Fun>::ArgsT::template Get<2>,
                    typename QkT::Function<Fun>::ArgsT::template Get<3>
                >)
        {
            MCT_ASSERT(m_Data != nullptr);

            using functorT = QkT::Function<Fun>;

            static_assert(std::is_same_v<typename functorT::ReturnT, void>, "Callback must return void");
            static_assert(functorT::ArgsT::Size == 4, "Callback must take 4 arguments: (x, y, z, Block)");

            using typeX     = typename functorT::ArgsT::template Get<0>;
            using typeY     = typename functorT::ArgsT::template Get<1>;
            using typeZ     = typename functorT::ArgsT::template Get<2>;
            using blockArgT = typename functorT::ArgsT::template Get<3>;

            static_assert(std::is_integral_v<typeX> && std::is_integral_v<typeY> && std::is_integral_v<typeZ>,
                "Coordinates must be integral types");

            static_assert(!std::is_same_v<blockArgT, T>,
                "Mutable ForEachXYZ should accept 'Block&' (or const Block&). Do not accept by value (copy).");

            Internal::ChunkLayout::ForEachIndexInSubchunk(
                [this, &fun](typeX x, typeY y, typeZ z, size_t idx)
                noexcept(std::is_nothrow_invocable_v<Fun, typeX, typeY, typeZ, blockArgT>)
                {
                    std::invoke(fun, x, y, z, m_Data[idx]);
                }
            );
        }

        template<typename Fun>
        constexpr void ForEachConstXYZ(Fun&& fun) const
                noexcept(std::is_nothrow_invocable_v<Fun,
                    typename QkT::Function<Fun>::ArgsT::template Get<0>,
                    typename QkT::Function<Fun>::ArgsT::template Get<1>,
                    typename QkT::Function<Fun>::ArgsT::template Get<2>,
                    typename QkT::Function<Fun>::ArgsT::template Get<3>
                >)
        {
            MCT_ASSERT(m_Data != nullptr);

            using functorT = QkT::Function<Fun>;

            static_assert(std::is_same_v<typename functorT::ReturnT, void>, "Callback must return void");
            static_assert(functorT::ArgsT::Size == 4, "Callback must take 4 arguments: (x, y, z, Block)");

            using typeX     = typename functorT::ArgsT::template Get<0>;
            using typeY     = typename functorT::ArgsT::template Get<1>;
            using typeZ     = typename functorT::ArgsT::template Get<2>;
            using blockArgT = typename functorT::ArgsT::template Get<3>;

            static_assert(std::is_integral_v<typeX> && std::is_integral_v<typeY> && std::is_integral_v<typeZ>,
                          "Coordinates must be integral types");

            static_assert(!std::is_same_v<blockArgT, T&>,
                          "Cannot bind mutable reference (T&) in ForEachConstXYZ. Use const T& or T.");

            Internal::ChunkLayout::ForEachIndexInSubchunk(
                [this, &fun](typeX x, typeY y, typeZ z, size_t idx)
                        noexcept(std::is_nothrow_invocable_v<Fun, typeX, typeY, typeZ, blockArgT>)
                {
                    std::invoke(fun, x, y, z, m_Data[idx]);
                }
            );
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

        template<typename Fun>
        constexpr void ForEachXYZ(Fun&& fun) 
                noexcept(std::is_nothrow_invocable_v<Fun,
                    typename QkT::Function<Fun>::ArgsT::template Get<0>,
                    typename QkT::Function<Fun>::ArgsT::template Get<1>,
                    typename QkT::Function<Fun>::ArgsT::template Get<2>,
                    typename QkT::Function<Fun>::ArgsT::template Get<3>
                >)
        {
            MCT_ASSERT(m_Data != nullptr);

            using functorT = QkT::Function<Fun>;

            static_assert(std::is_same_v<typename functorT::ReturnT, void>, "Callback must return void");
            static_assert(functorT::ArgsT::Size == 4, "Callback must take 4 arguments: (x, y, z, Block)");

            using typeX     = typename functorT::ArgsT::template Get<0>;
            using typeY     = typename functorT::ArgsT::template Get<1>;
            using typeZ     = typename functorT::ArgsT::template Get<2>;
            using blockArgT = typename functorT::ArgsT::template Get<3>;

            static_assert(std::is_integral_v<typeX> && std::is_integral_v<typeY> && std::is_integral_v<typeZ>,
                "Coordinates must be integral types");

            static_assert(!std::is_same_v<blockArgT, T>,
                "Mutable ForEachXYZ should accept 'Block&' (or const Block&). Do not accept by value (copy).");

            Internal::ChunkLayout::ForEachIndexInChunk(
                [this, &fun](typeX x, typeY y, typeZ z, size_t idx)
                    noexcept(std::is_nothrow_invocable_v<Fun, typeX, typeY, typeZ, blockArgT>)
                {
                    std::invoke(fun, x, y, z, m_Data[idx]);
                }
            );
        }

        template<typename Fun>
        constexpr void ForEachConstXYZ(Fun&& fun) const
                noexcept(std::is_nothrow_invocable_v<Fun,
                    typename QkT::Function<Fun>::ArgsT::template Get<0>,
                    typename QkT::Function<Fun>::ArgsT::template Get<1>,
                    typename QkT::Function<Fun>::ArgsT::template Get<2>,
                    typename QkT::Function<Fun>::ArgsT::template Get<3>
                >)
        {
            MCT_ASSERT(m_Data != nullptr);

            using functorT = QkT::Function<Fun>;

            static_assert(std::is_same_v<typename functorT::ReturnT, void>, "Callback must return void");
            static_assert(functorT::ArgsT::Size == 4, "Callback must take 4 arguments: (x, y, z, Block)");

            using typeX     = typename functorT::ArgsT::template Get<0>;
            using typeY     = typename functorT::ArgsT::template Get<1>;
            using typeZ     = typename functorT::ArgsT::template Get<2>;
            using blockArgT = typename functorT::ArgsT::template Get<3>;

            static_assert(std::is_integral_v<typeX> && std::is_integral_v<typeY> && std::is_integral_v<typeZ>,
                          "Coordinates must be integral types");

            static_assert(!std::is_same_v<blockArgT, T&>,
                          "Cannot bind mutable reference (T&) in ForEachConstXYZ. Use const T& or T.");

            Internal::ChunkLayout::ForEachIndexInChunk(
                [this, &fun](typeX x, typeY y, typeZ z, size_t idx)
                    noexcept(std::is_nothrow_invocable_v<Fun, typeX, typeY, typeZ, blockArgT>)
                {
                    std::invoke(fun, x, y, z, m_Data[idx]);
                }
            );
        }

    private:
        T* m_Data;
    };

}
