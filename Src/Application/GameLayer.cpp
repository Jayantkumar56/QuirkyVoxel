

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
		//m_World.Update(m_Player.GetPostion());

		//m_Renderer.Render(m_World, m_Player);
	}

}
