

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Renderer/Primitives/Types.h"
#include "Renderer/Primitives/BufferLayout.h"

#include <type_traits>


namespace Mct {

    using MeshGpuId = uint32_t;
    inline constexpr MeshGpuId NullMeshGpuId = static_cast<MeshGpuId>(-1);

    struct MeshUploadDesc {
        RenderPrimitive Primitive     = RenderPrimitive::Triangles;
        uint32_t        VertexCount   = 0;
        BufferLayout    VertexLayout; 
        uint32_t        IndexCount    = 0;
        IndexType       IndexType     = IndexType::None;
        size_t          InstanceCount = 1;
    };

    // Base for CPU-side mesh objects.
    class Mesh {
    public:
        Mesh(MeshUploadDesc&& uploadDescriptor)
            noexcept(std::is_nothrow_move_constructible_v<MeshUploadDesc>) : 
                m_UploadDescriptor(std::move(uploadDescriptor)) 
        {}

        virtual ~Mesh() = default;

        // GPU id is set on successful upload (render thread).
        constexpr MeshGpuId GetGpuId()       const noexcept { return m_GpuId; }
        constexpr void      SetGpuId(MeshGpuId id) noexcept { m_GpuId = id;   }

        // Descriptor describing layout/counts. Must be valid at upload time.
        constexpr const MeshUploadDesc& GetDesc() const noexcept { return m_UploadDescriptor; }

        // Raw data pointers. Must remain valid until UploadMesh() returns.
        // Returning nullptr is valid for missing buffer (e.g. instanced path with no index buffer).
        virtual const void* VertexData() const noexcept = 0;
        virtual const void* IndexData()  const noexcept = 0;

        // If mesh supports incremental streaming / dynamic updates, override this.
        // Renderer can use this to decide whether to keep a GPU-side mutable buffer.
        virtual bool IsDynamic() const noexcept { return false; }

    protected:
        MeshGpuId      m_GpuId = NullMeshGpuId;
        MeshUploadDesc m_UploadDescriptor;
    };

}
