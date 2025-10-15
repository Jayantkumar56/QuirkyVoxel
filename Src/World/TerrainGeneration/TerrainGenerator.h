

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldSettings.h"

#include <memory>


namespace Mct {

	class Chunk;

	class TerrainGenerator {
	public:
		[[nodiscard]] static std::unique_ptr<TerrainGenerator> Create(const TerrainType type);

		virtual ~TerrainGenerator() noexcept = default;

		virtual void GenerateFor(Chunk& chunk) noexcept = 0;
	};

}
