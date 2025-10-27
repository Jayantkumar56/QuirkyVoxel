

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "WorldSettings.h"
#include "ChunkManager.h"
#include "TerrainGeneration/TerrainGenerator.h"

#include <memory>


namespace Mct {

	class World {
	public:
		World(const WorldSettings& settings);

		void Update(glm::vec3 playerPos);

		[[nodiscard]] ChunkManager& GetChunkManager() noexcept { return m_ChunkManager; }

	private:
		std::unique_ptr<TerrainGenerator> m_TerrainGenerator;
		ChunkManager m_ChunkManager;
	};

}
