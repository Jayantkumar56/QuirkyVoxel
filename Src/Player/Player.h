

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "CameraController.h"


namespace Mct {

	class Event;

	class Player {
	public:
		Player();

		void OnEvent(Event& e);
		void OnUpdate(float deltaTime);

		const auto& GetCamera()   const { return m_CameraController.GetCamera();   }
		const auto  GetPosition() const { return m_CameraController.GetPosition(); }

	private:
		// TODO: move this to a proper serializer system and read from file.
		CameraController CreateController();

	private:
		CameraController m_CameraController;
	};

}
