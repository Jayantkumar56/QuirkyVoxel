

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "World.h"


namespace Mct {

	World::World(const WorldSettings& settings) :
			m_ChunkManager(TerrainGenerator::Create<SimpleTerrainGen>())
	{}

	void World::Update(glm::vec3 playerPos) {
		m_ChunkManager.Update(playerPos);
	}

}
