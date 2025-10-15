

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockStorage.h"
#include "World/Block/Block.h"


namespace Mct {

	class Subchunk {
	public:
		Subchunk(SubchunkSpan<Block> blocks) noexcept : m_Blocks(blocks) {}

		[[nodiscard]] SubchunkSpan<const Block> GetBlocks() const noexcept { return m_Blocks; }
		[[nodiscard]] SubchunkSpan<Block> GetBlocksForWrite()     noexcept { return m_Blocks; }

	private:
		SubchunkSpan<Block> m_Blocks;
	};

}
