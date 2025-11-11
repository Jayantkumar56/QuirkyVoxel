

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkCoord.h"
#include "Subchunk.h"
#include "BlockStorage.h"
#include "ChunkMesh.h"
#include "ChunkGpuMesh.h"

#include <glm/glm.hpp>

#include <span>
#include <memory>


namespace Mct {

	class Chunk {
		enum class MeshState {
			NoMesh,
			DirtyMesh,
			HaveMesh
		};

	public:
		Chunk(ChunkCoord coord) : 
				m_Coord  ( coord                            ),
				m_Blocks ( std::make_unique<BlockStorage>() )
		{
			m_Subchunks.reserve(WorldConst::SubchunkCount);

			glm::vec3 subchunkPos = m_Coord.ToBlockCoordFloat();

			for (size_t i = 0; i < WorldConst::SubchunkCount; ++i) {
				m_Subchunks.emplace_back(subchunkPos, m_Blocks->GetSubchunkForWrite(i));
				subchunkPos.y += static_cast<float>(static_cast<int>(WorldConst::SubchunkSizeY));
			}
		}

		[[nodiscard]] ChunkCoord GetCoord() const noexcept { return m_Coord; }

		[[nodiscard]] ChunkSpan<const Block> GetBlocks() const noexcept { return { m_Blocks->ViewForRead()  }; }
		[[nodiscard]] ChunkSpan<Block> GetBlocksForWrite()     noexcept { return { m_Blocks->ViewForWrite() }; }

		[[nodiscard]] std::span<const Subchunk> GetSubchunks() const noexcept { return m_Subchunks; }
		[[nodiscard]] std::span<Subchunk> GetSubchunksForWrite()     noexcept { return m_Subchunks; }

		[[nodiscard]] bool NeedRemesh()    const noexcept { return m_MeshState == MeshState::NoMesh;    }
		[[nodiscard]] bool HaveDirtyMesh() const noexcept { return m_MeshState == MeshState::DirtyMesh; }

		std::unique_ptr<ChunkMesh> GetMeshForUpload() noexcept { return std::move(m_CpuMesh); }

		void SetGpuMesh(std::unique_ptr<ChunkGpuMesh> mesh) {
			m_GpuMesh   = std::move(mesh);
			m_MeshState = MeshState::HaveMesh;

			for (size_t i = 0; i < WorldConst::SubchunkCount; ++i) {
				m_Subchunks[i].SetMesh(&m_GpuMesh->SubchunkMeshes[i]);
			}
		}

		void SetMesh(std::unique_ptr<ChunkMesh> cpuMesh) {
			m_CpuMesh   = std::move(cpuMesh);
			m_MeshState = MeshState::DirtyMesh;
		}

	private:
		ChunkCoord                    m_Coord;
		std::unique_ptr<BlockStorage> m_Blocks;
		std::vector<Subchunk>         m_Subchunks;

		MeshState m_MeshState = MeshState::NoMesh;

		std::unique_ptr<ChunkMesh>    m_CpuMesh;
		std::unique_ptr<ChunkGpuMesh> m_GpuMesh;
	};

}
