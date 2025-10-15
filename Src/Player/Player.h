

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>


namespace Mct {

	class Player {
	public:
		Player(glm::vec3 position) : m_Position(position) {}

		[[nodiscard]] glm::vec3 GetPostion() const noexcept { return m_Position; }

	private:
		glm::vec3 m_Position;
	};

}
