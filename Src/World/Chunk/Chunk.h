

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkCoord.h"
#include "Subchunk.h"
#include "BlockStorage.h"
#include "World/WorldConstants.h"
#include "World/Block/Block.h"

#include <glm/glm.hpp>

#include <span>
#include <vector>


namespace Mct {

	class Chunk {
	public:
		Chunk(ChunkCoord coord) : 
				m_Coord(coord),
				m_Blocks(std::make_unique<BlockStorage>())
		{
			m_Subchunks.reserve(WorldConst::SubchunkCount);

			for (size_t i = 0; i < WorldConst::SubchunkCount; ++i) {
				m_Subchunks.emplace_back(m_Blocks->SubchunkView(i));
			}
		}

		[[nodiscard]] ChunkCoord GetCoord() const noexcept { m_Coord; }

		[[nodiscard]] ChunkSpan<const Block> GetBlocks() const noexcept { return { m_Blocks->View() }; }
		[[nodiscard]] ChunkSpan<Block> GetBlocksForWrite()     noexcept { return { m_Blocks->View() }; }

		[[nodiscard]] std::span<const Subchunk> GetSubchunks() const noexcept { return m_Subchunks; }
		[[nodiscard]] std::span<Subchunk> GetSubchunksForWrite()     noexcept { return m_Subchunks; }

	private:
		ChunkCoord                    m_Coord;
		std::unique_ptr<BlockStorage> m_Blocks;
		std::vector<Subchunk>         m_Subchunks;
	};

}
