

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GameLayer.h"
#include "World/WorldSettings.h"

#include "imgui.h"


namespace Mct {

	GameLayer::GameLayer() :
			m_Player(glm::vec3(0.0f)),
			m_World(WorldSettings{ TerrainType::SuperFlat })
	{}

	void GameLayer::OnUpdate() {
		{
			ImGui::Begin("Hello, world!");
			ImGuiIO& io = ImGui::GetIO();

			ImGui::Text("This is some useful text.");

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		//m_World.Update(m_Player.GetPostion());

		//m_Renderer.Render(m_World, m_Player);
	}

	void GameLayer::OnEvent(Event& e) {

	}

}
