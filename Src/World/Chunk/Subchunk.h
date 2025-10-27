

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/Block/Block.h"
#include "ChunkSpan.h"
#include "Renderer/Mesh/GpuMesh.h"


namespace Mct {

	class Subchunk {
	public:
		Subchunk(SubchunkSpan<Block> blocks) noexcept : m_Blocks(blocks) {}

		[[nodiscard]] SubchunkSpan<const Block> GetBlocks() const noexcept { return m_Blocks; }
		[[nodiscard]] SubchunkSpan<Block> GetBlocksForWrite()     noexcept { return m_Blocks; }

		void SetSolidMesh(GpuMesh solidMesh) { m_SolidMesh = solidMesh; }
		void SetWaterMesh(GpuMesh waterMesh) { m_WaterMesh = waterMesh; }

	private:
		SubchunkSpan<Block> m_Blocks;

		std::optional<GpuMesh> m_SolidMesh;
		std::optional<GpuMesh> m_WaterMesh;
	};

}
