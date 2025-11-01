

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Camera.h"


namespace Mct {

	class Event;

	class KeyPressedEvent;
	class KeyReleasedEvent;
	
	struct CameraControls {
		int Forward;
		int Backward;
		int Right;
		int Left;
		int Up;
		int Down;
	};

	struct ControllerSpecs {
		float MaxSpeed;
		float Acceleration;
		float Deceleration;
	};

	class CameraController {
	public:
		CameraController(const PerspectiveCameraSpecs& camSpec, 
						 const CameraControls&         controls, 
						 const ControllerSpecs&        controllerSpec) :
				m_Up             ( camSpec.Up     ),
				m_Velocity       ( 0.0f           ),
				m_MousePos       ( 0.0f           ),
				m_MouseSpeed     ( 5.0f           ),
				m_Camera         ( camSpec        ),
				m_CameraControls ( controls       ),
				m_ControllerSpec ( controllerSpec )
		{}

		void OnEvent(Event& e);
		void OnUpdate(float deltaTime);

		const auto& GetCamera()   const noexcept { return m_Camera;               }
		const auto  GetPosition() const noexcept { return m_Camera.GetPosition(); }

		void OnWindowResized(uint16_t width, uint16_t height) { 
			m_Camera.SetAspectRatio(float(width) / float(height)); 
		}

	private:
		void CalculateVelocity(float deltaTime);
		void CalcCameraOrientation();
		glm::vec3 CalculateAcceleration() const;

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);

	private:
		glm::vec3 m_Up;
		glm::vec3 m_Velocity;
		glm::vec2 m_MousePos;
		float     m_MouseSpeed;

		PerspectiveCamera  m_Camera;
		CameraControls     m_CameraControls;
		ControllerSpecs    m_ControllerSpec;

		bool m_IsMovingForward  = false;
		bool m_IsMovingBackward = false;
		bool m_IsMovingLeft     = false;
		bool m_IsMovingRight    = false;
		bool m_IsMovingUp       = false;
		bool m_IsMovingDown     = false;
	};

}
