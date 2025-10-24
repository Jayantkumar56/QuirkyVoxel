

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Chunk/Subchunk.h"


namespace Mct {

	class ChunkManager {
	public:
		static const Subchunk& GetEmptySubchunk() noexcept { return s_EmptySubchunk; }

	private:
		static Subchunk s_EmptySubchunk;
		static std::array<Block, WorldConst::SubchunkBlockCount> s_EmptySubchunkData;
	};

}
