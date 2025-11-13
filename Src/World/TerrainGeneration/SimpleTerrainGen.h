

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldConstants.h"

#include <array>


namespace Mct {

	class Chunk;

	class SimpleTerrainGen {
		using HeightMap = std::array<std::array<float, WorldConst::ChunkSizeZ>, WorldConst::ChunkSizeX>;

	public:
		void GenerateFor(Chunk& chunk);

	private:
		HeightMap CreateHeightMap(Chunk& chunk);
	};

}
