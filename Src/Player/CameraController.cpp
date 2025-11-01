

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "CameraController.h"
#include "Events/KeyboardEvents.h"
#include "Events/EventDispatcher.h"

#include <GLFW/glfw3.h>


namespace Mct {

	void CameraController::OnEvent(Event& e) {
		EventDispatcher::Handle<KeyPressedEvent>(e,  [this](KeyPressedEvent& e)  { return OnKeyPressed(e);  });
		EventDispatcher::Handle<KeyReleasedEvent>(e, [this](KeyReleasedEvent& e) { return OnKeyReleased(e); });
	}
	
	void CameraController::OnUpdate(float deltaTime) {
		CalculateVelocity(deltaTime);

		glm::vec3 posOffset = m_Velocity * deltaTime;

		if (posOffset != glm::vec3(0.0f)) {
			m_Camera.AddMovementInput(posOffset);
		}

		//if (Quirk::Input::IsKeyPressed(QK_Key_LeftMouseBtn))
		//	CalcCameraOrientation();

		//m_MousePos.x = Quirk::Input::MouseCurrentX();
		//m_MousePos.y = Quirk::Input::MouseCurrentY();
	}

	void CameraController::CalcCameraOrientation() {
		//glm::vec2 mousePos   ( Quirk::Input::MouseCurrentX(),  Quirk::Input::MouseCurrentY()  );
		//glm::vec2 deltaMouse ( mousePos - m_MousePos );

		//deltaMouse *= m_MouseSpeed * Quirk::Time::GetDeltaTime();

		//m_Camera.AddYawInput(deltaMouse.x);
		//m_Camera.AddPitchInput(deltaMouse.y);
	}

	void CameraController::CalculateVelocity(float deltaTime) {
		glm::vec3 acceleration = CalculateAcceleration();

		if (acceleration == glm::vec3(0.0f)) {
			m_Velocity -= m_Velocity * std::min(deltaTime * m_ControllerSpec.Deceleration, 1.0f);
		}
		else {
			m_Velocity += acceleration * m_ControllerSpec.Acceleration * deltaTime;

			if (glm::length(m_Velocity) > m_ControllerSpec.MaxSpeed)
				m_Velocity = glm::normalize(m_Velocity) * m_ControllerSpec.MaxSpeed;
		}
	}

	glm::vec3 CameraController::CalculateAcceleration() const {
		glm::vec3 acceleration(0.0f);

		glm::vec3 right   = m_Camera.GetRight();
		glm::vec3 up      = m_Up;
		glm::vec3 forward = glm::cross(up, right);
		

		if ( m_IsMovingForward  ) { acceleration += forward;  }
		if ( m_IsMovingBackward ) { acceleration += -forward; }
		if ( m_IsMovingLeft     ) { acceleration += right;    }
		if ( m_IsMovingRight    ) { acceleration += -right;   }
		if ( m_IsMovingUp       ) { acceleration += up;       }
		if ( m_IsMovingDown     ) { acceleration -= up;       }

		return acceleration;
	}

	bool CameraController::OnKeyPressed(KeyPressedEvent& e) {
		int key = e.GetKeyCode();
    
		if ( key == m_CameraControls.Forward  ) { m_IsMovingForward  = true; return true; }
		if ( key == m_CameraControls.Backward ) { m_IsMovingBackward = true; return true; }
		if ( key == m_CameraControls.Left     ) { m_IsMovingLeft     = true; return true; }
		if ( key == m_CameraControls.Right    ) { m_IsMovingRight    = true; return true; }
		if ( key == m_CameraControls.Up       ) { m_IsMovingUp       = true; return true; }
		if ( key == m_CameraControls.Down     ) { m_IsMovingDown     = true; return true; }

		return false;
	}

	bool CameraController::OnKeyReleased(KeyReleasedEvent& e) {
		int key = e.GetKeyCode();

		if ( key == m_CameraControls.Forward  ) { m_IsMovingForward  = false; return true; }
		if ( key == m_CameraControls.Backward ) { m_IsMovingBackward = false; return true; }
		if ( key == m_CameraControls.Left     ) { m_IsMovingLeft     = false; return true; }
		if ( key == m_CameraControls.Right    ) { m_IsMovingRight    = false; return true; }
		if ( key == m_CameraControls.Up       ) { m_IsMovingUp       = false; return true; }
		if ( key == m_CameraControls.Down     ) { m_IsMovingDown     = false; return true; }

		return false;
	}

}
