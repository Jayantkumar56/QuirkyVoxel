

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>


namespace Mct {

	// The 8 corners of a unit cube (0,0,0) to (1,1,1).
    inline constexpr glm::u8vec3 CUBE_CORNERS[8] = {
        {0, 0, 0},    // 0: -X, -Y, -Z ( Near Bottom Left  )
        {1, 0, 0},    // 1: +X, -Y, -Z ( Near Bottom Right )
        {1, 1, 0},    // 2: +X, +Y, -Z ( Near Top Right    )
        {0, 1, 0},    // 3: -X, +Y, -Z ( Near Top Left     )
        {0, 0, 1},    // 4: -X, -Y, +Z ( Far Bottom Left   )
        {1, 0, 1},    // 5: +X, -Y, +Z ( Far Bottom Right  )
        {1, 1, 1},    // 6: +X, +Y, +Z ( Far Top Right     )
        {0, 1, 1}     // 7: -X, +Y, +Z ( Far Top Left      )
    };

    // Indices into CUBE_CORNERS for each face (6 faces * 4 corners = 24 indices).
    // The order defines the quad vertices v1, v2, v3, v4 for AddFace.
    // IMPORTANT: The winding order must be correct for each face normal.
    inline constexpr glm::u8vec4 CUBE_FACE_INDICES[6] = {
        // normID, v1, v2, v3, v4 (Indices into CUBE_CORNERS).
        { /* Top    (+Y) */ 3, 7, 6, 2 }, // Winding: Front face looking down  -Y
        { /* Bottom (-Y) */ 0, 1, 5, 4 }, // Winding: Front face looking up    +Y
        { /* East   (+X) */ 1, 2, 6, 5 }, // Winding: Front face looking along -X
        { /* West   (-X) */ 4, 7, 3, 0 }, // Winding: Front face looking along +X
        { /* South  (+Z) */ 5, 6, 7, 4 }, // Winding: Front face looking along -Z
        { /* North  (-Z) */ 0, 3, 2, 1 }  // Winding: Front face looking along +Z
    };

    enum class CubeNormal : uint8_t {
        Top    = 0, 
        Bottom = 1,
        East   = 2, 
        West   = 3, 
        South  = 4, 
        North  = 5, 
        COUNT
    };

    // Maps NormalDirection enum values to their corresponding NEIGHBOR offset vector.
    inline constexpr glm::i8vec3 CUBE_NEIGHBOR_OFFSETS[static_cast<size_t>(CubeNormal::COUNT)] = {
        { 0,  1,  0},   // Top    (+Y)
        { 0, -1,  0},   // Bottom (-Y)
        { 1,  0,  0},   // East   (+X)
        {-1,  0,  0},   // West   (-X)
        { 0,  0,  1},   // South  (+Z)
        { 0,  0, -1}    // North  (-Z)
    };

}
