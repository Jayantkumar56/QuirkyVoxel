

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GameLayer.h"
#include "World/WorldSettings.h"


namespace Mct {

	GameLayer::GameLayer() :
			m_Player(glm::vec3(0.0f)),
			m_World(WorldSettings{ TerrainType::SuperFlat })
	{}

	void GameLayer::OnUpdate() {

	}

}
