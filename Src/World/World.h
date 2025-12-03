

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "WorldSettings.h"
#include "ChunkManager.h"
#include "Sky.h"
#include "WorldTime.h"

#include <memory>


namespace Mct {

	class World {
	public:
		World(const WorldSettings& settings);

		void Update(float deltaTime, glm::vec3 playerPos);

		[[nodiscard]] ChunkManager& GetChunkManager() noexcept {
			return m_ChunkManager;
		}

		[[nodiscard]] const Sky& GetSky() const noexcept {
			return m_Sky;
		}

		[[nodiscard]] glm::vec3 GetSunDirection() const noexcept {
			return m_Sky.GetSunDirection();
		}

		[[nodiscard]] WorldTime& GetGameTime() noexcept {
			return m_GameTime;
		}

	private:
		Sky          m_Sky;
		WorldTime    m_GameTime;
		ChunkManager m_ChunkManager;
	};

}
