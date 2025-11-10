

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "MeshManager.h"


namespace Mct {

	MeshManager::MeshManager(const BufferLayout vboLayout,
                             const size_t vertexBufferSize,
                             const size_t indexBufferSize,
                             const size_t vertexMinBlockSize,
                             const size_t indexMinBlockSize) : 
            m_VertexBufferSize ( vertexBufferSize                                        ),
            m_IndexBufferSize  ( indexBufferSize                                         ),
            m_VertexBuffer     ( std::make_shared<VertexBuffer>((uint32_t)vertexBufferSize)        ),
            m_IndexBuffer      ( std::make_shared<IndexBuffer>(nullptr, (uint32_t)indexBufferSize) ),
            m_VertexAllocator  ( vertexBufferSize, vertexMinBlockSize                    ),
            m_IndexAllocator   ( indexBufferSize, indexMinBlockSize                      ) 
    {
        m_VertexBuffer->SetLayout(vboLayout);
        m_VertexArray.SetIndexBuffer(m_IndexBuffer);
		m_VertexArray.AddVertexBuffer(m_VertexBuffer);
    }

	std::optional<GpuMesh> MeshManager::UploadMesh(const Mesh& mesh) {
        const MeshUploadDesc& desc = mesh.GetDesc();

        // Handle Empty Mesh
        // If there are no vertices, return a "null" handle.
        if (desc.VertexCount == 0) {
            return std::nullopt;
        }

        // Calculate Byte Sizes
        const void* vertexData = mesh.VertexData();
        MCT_ASSERT(vertexData != nullptr && "Mesh has VertexCount > 0 but VertexData is null!");

        const size_t vertexDataSize = desc.VertexCount * desc.VertexLayout.GetStride();

        const void* indexData = mesh.IndexData();
        const size_t indexDataSize = (indexData != nullptr)
            ? (desc.IndexCount * sizeof(uint32_t))
            : 0;


        // Allocate Vertex Buffer
        auto vOffsetOpt = m_VertexAllocator.Allocate(vertexDataSize);
        MCT_ASSERT(vOffsetOpt && "MeshManager: Failed to allocate vertex buffer space!");

        const size_t vOffset = *vOffsetOpt; // Byte offset

        // Allocate Index Buffer
        std::optional<IndexBufferHandle> iboHandle;
        if (indexDataSize > 0) {
            auto iOffsetOpt = m_IndexAllocator.Allocate(indexDataSize);
            MCT_ASSERT(iOffsetOpt && "MeshManager: Failed to allocate index buffer space!");

            const size_t iOffset = *iOffsetOpt;

            // Upload Index Data
            UploadIndexData(indexData, indexDataSize, iOffset);

            // Create the handle
            iboHandle.emplace(iOffset, indexDataSize, desc.IndexType);
        }

        // Upload Vertex Data
        UploadVertexData(vertexData, vertexDataSize, vOffset);

        // Create the handle
        VertexBufferHandle vboHandle(vOffset, vertexDataSize, m_VBOLayoutId);

        // Return the GpuMesh
        return GpuMesh{
            .VboHandle = vboHandle,
            .IboHandle = iboHandle
        };
	}

	void MeshManager::UploadVertexData(const void* data, size_t size, size_t offset) noexcept {
        if (m_VertexBuffer) {
            m_VertexBuffer->Upload(data, size, offset);
        }
	}

	void MeshManager::UploadIndexData(const void* data, size_t size, size_t offset) noexcept {
        if (m_IndexBuffer) {
            m_IndexBuffer->Upload(data, size, offset);
        }
	}

    void MeshManager::FreeMesh(const GpuMesh& mesh) {
        if (mesh.VboHandle.GetSize())
            m_VertexAllocator.Free(mesh.VboHandle.GetOffset());

        // 2. Free the index buffer block, if it exists
        if (mesh.IboHandle && mesh.IboHandle.value().GetSize()) {
            m_IndexAllocator.Free(mesh.IboHandle->GetOffset());
        }
    }

}
