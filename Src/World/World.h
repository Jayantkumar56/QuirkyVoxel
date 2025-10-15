

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "WorldSettings.h"
#include "TerrainGeneration/TerrainGenerator.h"

#include <memory>


namespace Mct {

	struct WorldSettings;

	class World {
	public:
		World(const WorldSettings& settings);

	private:
		std::unique_ptr<TerrainGenerator> m_TerrainGenerator;
	};

}
