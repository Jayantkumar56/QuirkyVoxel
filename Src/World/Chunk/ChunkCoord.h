

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/Math.h"
#include "World/WorldConstants.h"

#include <glm/glm.hpp>

#include <compare>


namespace Mct {

    struct ChunkCoord {
        int X;
        int Z;

        // Map world (block) XZ to ChunkCoord.
        [[nodiscard]] static constexpr ChunkCoord FromWorldXZ(const int wx, const int wz) noexcept {
            return {
                .X{ Utils::FloorDiv(wx, static_cast<int>(WorldConst::ChunkSizeX)) },
                .Z{ Utils::FloorDiv(wz, static_cast<int>(WorldConst::ChunkSizeZ)) }
            };
        }

        // Lexicographical ordering: compares X first, then Z if X is equal.
        [[nodiscard]] constexpr auto operator<=>(const ChunkCoord&) const noexcept = default;

        [[nodiscard]] constexpr glm::ivec3 ToBlockCoord() const noexcept {
            return glm::ivec3{
                X * static_cast<int>(WorldConst::ChunkSizeX),
                0,
                Z * static_cast<int>(WorldConst::ChunkSizeZ)
            };
        }

        [[nodiscard]] constexpr glm::vec3 ToBlockCoordFloat() const noexcept {
            return glm::vec3{
                static_cast<float>(X * static_cast<int>(WorldConst::ChunkSizeX)),
                0.0f,
                static_cast<float>(Z * static_cast<int>(WorldConst::ChunkSizeZ))
            };
        }
    };

}

namespace std {

    template <>
    struct hash<Mct::ChunkCoord> {
        size_t operator()(const Mct::ChunkCoord& c) const noexcept {
            size_t hx = std::hash<int>()(c.X);
            size_t hz = std::hash<int>()(c.Z);

            // Combine the hashes.
            // This formula is inspired by boost::hash_combine
            // and is designed to create a good distribution. 
            // The "magic number" is derived from the golden ratio.
            return hx ^ (hz + 0x9e3779b97f4a7c15ull + (hx << 6) + (hx >> 2));
        }
    };

}
