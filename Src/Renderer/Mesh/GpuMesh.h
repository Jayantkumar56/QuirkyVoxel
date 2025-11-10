

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Renderer/Primitives/BufferHandle.h"

#include <optional>


namespace Mct {

    struct GpuMesh {
        // Handle to the block of memory in the massive VBO.
        VertexBufferHandle VboHandle;

        // Handle to the block of memory in the massive IBO.
        // This is optional, as a mesh may not be indexed.
        std::optional<IndexBufferHandle> IboHandle;
    };

}
