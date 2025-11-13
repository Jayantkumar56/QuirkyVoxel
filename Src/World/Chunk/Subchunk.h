

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/Block/Block.h"
#include "ChunkSpan.h"
#include "ChunkGpuMesh.h"

#include <glm/glm.hpp>


namespace Mct {

	class Subchunk {
	public:
		Subchunk(glm::vec3 pos, SubchunkSpan<Block> blocks) noexcept : 
				m_Position ( pos    ),
				m_Blocks   ( blocks ) 
		{}

		[[nodiscard]] SubchunkSpan<const Block> GetBlocks() const noexcept { return m_Blocks; }
		[[nodiscard]] SubchunkSpan<Block> GetBlocksForWrite()     noexcept { return m_Blocks; }

		void SetMesh(SubchunkGpuMesh* mesh) noexcept { m_Mesh = mesh; }

		bool HasSolidMesh() const noexcept { return m_Mesh && m_Mesh->SolidMesh.has_value(); }
		const GpuMeshHandle& GetSolidMesh() const noexcept { return *m_Mesh->SolidMesh; }

		const glm::vec3& GetPosition() const noexcept { return m_Position; }

	private:
		glm::vec3           m_Position;
		SubchunkSpan<Block> m_Blocks;

		// Points to the Gpu mesh object for this subchunk stored in the chunk.
		SubchunkGpuMesh* m_Mesh = nullptr;
	};

}
