

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "World.h"


namespace Mct {

	World::World(const WorldSettings& settings) :
		m_TerrainGenerator(TerrainGenerator::Create(settings.TerrainType))
	{}

}
