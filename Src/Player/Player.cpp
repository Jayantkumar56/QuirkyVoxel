

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Player.h"

#include <GLFW/glfw3.h>


namespace Mct {

    Player::Player() : 
			m_CameraController(CreateController())
    {}

	void Player::OnEvent(Event& e) {
		m_CameraController.OnEvent(e);
	}

	void Player::OnUpdate(float deltaTime) {
		m_CameraController.OnUpdate(deltaTime);
	}

    CameraController Player::CreateController() {
		return CameraController(
			PerspectiveCameraSpecs{
				.FOVY        { glm::radians(45.0f)           },
				.AspectRatio { 1.0f                          },
				.ZNear       { 1.0f                          },
				.ZFar        { 1000.0f                       },
				.Position    { glm::vec3(8.0f, 100.0f, 8.0f) },
				.Up          { glm::vec3(0.0f, 1.0f, 0.0f)   },
				.Forward     { glm::vec3(0.0f, 0.0f, 1.0f)   },
			},
			CameraControls{
				.Forward { GLFW_KEY_W     }, .Backward { GLFW_KEY_S            },
				.Right   { GLFW_KEY_D     }, .Left     { GLFW_KEY_A            },
				.Up      { GLFW_KEY_SPACE }, .Down     { GLFW_KEY_LEFT_CONTROL }
			},
			ControllerSpecs{
				.MaxSpeed     { 25.0f  },
				.Acceleration { 10.0f  },
				.Deceleration { 15.0f  }
			}
		);
    }

}
