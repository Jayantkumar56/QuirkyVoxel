

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <cinttypes>


namespace Mct {

    enum class IndexType : uint8_t {
        None,
        UInt16,
        UInt32
    };

    enum class RenderPrimitive : uint8_t {
        Lines,
        Triangles,
        Quads
    };

}
