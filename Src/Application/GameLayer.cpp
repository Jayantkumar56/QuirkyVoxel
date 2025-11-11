

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GameLayer.h"
#include "Application/Window.h"
#include "Application/Application.h"
#include "World/WorldSettings.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/EventDispatcher.h"
#include "Events/ApplicationEvents.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>


namespace Mct {

	GameLayer::GameLayer() :
			m_Player(),
			m_World(WorldSettings{ TerrainType::SuperFlat })
	{}

	void GameLayer::OnAttach() {
		auto& window = GetApp()->GetWindow();
		const float width  = static_cast<float>(window.GetWidth());
		const float height = static_cast<float>(window.GetHeight());

		m_Player.GetCamera().SetAspectRatio(width / height);
		m_Renderer.OnViewportResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
	}

	void GameLayer::OnEvent(Event& e) {
		EventDispatcher::Handle<KeyPressedEvent>(e, [this](KeyPressedEvent& event) {
			if (event.GetKeyCode() == GLFW_KEY_ESCAPE) {
				m_GameHaveFocus = false;
				GetApp()->GetWindow().SetCursorMode(GLFW_CURSOR_NORMAL);
				return true;
			}

			return false;
		});

		EventDispatcher::Handle<MouseButtonPressedEvent>(e, [this](MouseButtonPressedEvent& event) {
			if (event.GetButton() == GLFW_MOUSE_BUTTON_LEFT) {
				m_GameHaveFocus = true;
				GetApp()->GetWindow().SetCursorMode(GLFW_CURSOR_DISABLED);
				return true;
			}
			return false;
		});

		EventDispatcher::Handle<WindowResizeEvent>(e, [this](WindowResizeEvent& event) {
			const float width  = static_cast<float>(event.GetWidth());
			const float height = static_cast<float>(event.GetHeight());

			// Don't update if window is minimized
			if (width <= 0.0f || height <= 0.0f) {
				return false;
			}

			m_Player.GetCamera().SetAspectRatio(width / height);
			m_Renderer.OnViewportResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

			return false;
		});

		if (m_GameHaveFocus) {
			m_Player.OnEvent(e);
		}
	}

	void GameLayer::OnUpdate(float deltaTime) {
		UpdateGameUI(deltaTime);
		UpdateDebugUI(deltaTime);
	}

	void GameLayer::UpdateGameUI(float deltaTime) {
		// Updates game state ony when game window have focus.
		if (m_GameHaveFocus) {
			m_Player.OnUpdate(deltaTime);
			m_World.Update(m_Player.GetPosition());

			m_Renderer.Render(m_World, m_Player);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

		ImGui::Begin("Game");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_GameViewportSize.x != viewportPanelSize.x || m_GameViewportSize.y != viewportPanelSize.y) {
			/*m_Renderer.OnViewportResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);

			m_GameViewportSize = { viewportPanelSize.x, viewportPanelSize.y };*/

			// TODO: update camera's aspect ratio
			// m_Player.GetCamera().SetAspectRatio(viewportPanelSize.x / viewportPanelSize.y);
		}

		uint32_t textureID = m_Renderer.GetFinalTextureID();

		// Note: ImGui's Y-axis is inverted for UVs, so we pass {0, 1} and {1, 0}
		ImGui::Image((void*)(intptr_t)textureID,
			ImVec2{ m_GameViewportSize.x, m_GameViewportSize.y },
			ImVec2{ 0, 1 }, // Top-left UV
			ImVec2{ 1, 0 }  // Bottom-right UV
		);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void GameLayer::UpdateDebugUI(float deltaTime) {
		ImGui::Begin("Hello, world!");
		ImGuiIO& io = ImGui::GetIO();

		glm::vec3 player = m_Player.GetPosition();

		ImGui::Text("Player Position:");
		ImGui::InputFloat3("##pos", glm::value_ptr(player));

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

}
