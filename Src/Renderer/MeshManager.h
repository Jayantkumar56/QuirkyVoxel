

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/NonCopyable.h"
#include "Common/BuddyAllocator.h"
#include "Primitives/VertexBuffer.h"
#include "Primitives/IndexBuffer.h"
#include "Primitives/VertexArray.h"
#include "Mesh/Mesh.h"
#include "Mesh/GpuMesh.h"

#include <memory>
#include <optional>


namespace Mct {

    // Manages GPU buffers and memory allocation for meshes
    // that all share a single, predefined vertex layout.
    //
    class MeshManager : public NonCopyable {
    public:
        MeshManager(const BufferLayout vboLayout,
                    const size_t vertexBufferSize,
                    const size_t indexBufferSize,
                    const size_t vertexMinBlockSize,
                    const size_t indexMinBlockSize);

        // Uploads a CPU-side mesh to the GPU. Returns std::nullopt on allocation/upload failure.
        [[nodiscard]] std::optional<GpuMesh> UploadMesh(const Mesh& mesh);

        void FreeMesh(const GpuMesh& mesh);

        [[nodiscard]] const VertexArray& GetCommonVAO() const noexcept { return m_VertexArray; }

        [[nodiscard]] size_t GetVertexBufferSize() const noexcept { return m_VertexBufferSize; }
        [[nodiscard]] size_t GetIndexBufferSize()  const noexcept { return m_IndexBufferSize;  }

        [[nodiscard]] const BufferLayout& GetVBOLayout() const noexcept { return m_VertexBuffer->GetLayout(); }

    private:
        void UploadVertexData(const void* data, size_t size, size_t offset) noexcept;
        void UploadIndexData(const void* data, size_t size, size_t offset)  noexcept;

    private:
        size_t m_VertexBufferSize{};
        size_t m_IndexBufferSize{};

        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        VertexArray  m_VertexArray;

        BuddyAllocator m_VertexAllocator;
        BuddyAllocator m_IndexAllocator;

        BufferLayoutId m_VBOLayoutId = 0;
    };

}
