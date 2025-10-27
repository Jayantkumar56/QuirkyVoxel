

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "MeshManager.h"


namespace Mct {

	MeshManager::MeshManager(const BufferLayout vboLayout, 
							 const size_t vertexBufferSize, 
							 const size_t indexBufferSize, 
							 const size_t vertexMinBlockSize, 
							 const size_t indexMinBlockSize) noexcept : 
		m_IndexBuffer(nullptr, indexBufferSize),
		m_IndexAllocator(indexBufferSize, 64),
		m_VertexAllocator(vertexBufferSize, 64)
	{}

	std::optional<GpuMesh> MeshManager::UploadMesh(const Mesh& mesh) noexcept {
		return std::optional<GpuMesh>();
	}

	void MeshManager::UploadVertexData(const void* data, size_t size, size_t offset) noexcept {

	}

	void MeshManager::UploadIndexData(const void* data, size_t size, size_t offset) noexcept {

	}

}
