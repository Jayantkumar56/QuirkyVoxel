

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"
#include "Renderer/Primitives/BufferHandle.h"

#include <optional>


namespace Mct {

    //struct GpuMeshHandle {
    //    // Handle to the block of memory in the massive VBO.
    //    VertexBufferHandle VboHandle;

    //    // Handle to the block of memory in the massive IBO.
    //    // This is optional, as a mesh may not be indexed.
    //    std::optional<IndexBufferHandle> IboHandle;
    //};

    class MeshManager;

    class GpuMeshHandle : public NonCopyable {
    public:
        // Handle to the block of memory in the massive VBO.
        VertexBufferHandle VboHandle;

        // Handle to the block of memory in the massive IBO.
        // This is optional, as a mesh may not be indexed.
        std::optional<IndexBufferHandle> IboHandle;

        // It can only be created by its manager, which passes a pointer to itself.
        GpuMeshHandle(MeshManager*                     manager,
                      VertexBufferHandle               vbo, 
                      std::optional<IndexBufferHandle> ibo);

        ~GpuMeshHandle();

        GpuMeshHandle(GpuMeshHandle&& other) noexcept;
        GpuMeshHandle& operator=(GpuMeshHandle&& other) noexcept;

    private:
        // Pointer back to the object responsible for deallocation.
        MeshManager* m_Manager;
    };

}
