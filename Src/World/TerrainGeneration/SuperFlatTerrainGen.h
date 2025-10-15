

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "TerrainGenerator.h"


namespace Mct {

	class Chunk;

	class SuperFlatTerrainGen : public TerrainGenerator {
	public:
		virtual void GenerateFor(Chunk& chunk) noexcept;
	};

}
