

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>


namespace Mct {

	class Event;

	// TODO: Remove this and create a dedicated camera class
	class Camera {};

	class Player {
	public:
		Player(glm::vec3 position) : 
				m_Position(position) 
		{}

		void OnUpdate();
		bool OnEvent(Event& e);

		[[nodiscard]] glm::vec3 GetPostion() const noexcept { return m_Position; }

		[[nodiscard]] const Camera& GetCamera() noexcept { return m_Camera; }

	private:
		Camera    m_Camera;
		glm::vec3 m_Position;
	};

}
