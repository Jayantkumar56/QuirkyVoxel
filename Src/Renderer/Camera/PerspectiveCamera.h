

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Mct {

	struct PerspectiveCameraSpecs {
		float FOVY;
		float AspectRatio;         // (width / height)
		float ZNear;
		float ZFar;
		glm::vec3 Position;
		glm::vec3 Up;
		glm::vec3 Forward;
	};

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(const PerspectiveCameraSpecs& spec) noexcept :
				m_FOVY                 ( spec.FOVY        ),
				m_AspectRatio          ( spec.AspectRatio ),
				m_ZNear                ( spec.ZNear       ),
				m_ZFar                 ( spec.ZFar        ),
				m_Position             ( spec.Position    ),
				m_Orientation          ( glm::quatLookAt(spec.Forward, spec.Up) ),
				m_ViewMatrix           ( glm::mat4_cast(glm::conjugate(m_Orientation)) * glm::translate(glm::mat4(1.0f), -m_Position) ),
				m_ProjectionMatrix     ( glm::perspective(spec.FOVY, spec.AspectRatio, spec.ZNear, spec.ZFar)                         ),
				m_ViewProjectionMatrix ( m_ProjectionMatrix * m_ViewMatrix )
		{}

		virtual glm::mat4 GetViewProjection() const noexcept override { return m_ViewProjectionMatrix; }
		virtual glm::vec3 GetPosition()       const noexcept override { return m_Position;             }

		glm::mat4 GetViewMatrix()     const noexcept { return m_ViewMatrix;           }
		glm::quat GetOrientation()    const noexcept { return m_Orientation;          }
		glm::vec3 GetRight()          const noexcept { return glm::normalize(m_Orientation * glm::vec3(1, 0, 0));  }
		glm::vec3 GetForward()        const noexcept { return glm::normalize(m_Orientation * glm::vec3(0, 0, -1)); }
		glm::vec3 GetUp()             const noexcept { return glm::normalize(m_Orientation * glm::vec3(0, 1, 0));  }

		void SetOrientation(const glm::quat orientation) noexcept {
			m_Orientation = orientation;
			UpdateViewAndViewProjection();
		}

		void AddMovementInput(const glm::vec3& offset) noexcept {
			m_Position += offset;
			UpdateViewAndViewProjection();
		}

		void AddPitchInput(const float offset) noexcept {
			m_Orientation = glm::normalize(glm::angleAxis(glm::radians(-offset), GetRight()) * m_Orientation);
			UpdateViewAndViewProjection();
		}

		void AddYawInput(const float offset) noexcept {
			m_Orientation = glm::normalize(glm::angleAxis(glm::radians(-offset), glm::vec3(0, 1, 0)) * m_Orientation);
			UpdateViewAndViewProjection();
		}

		void SetAspectRatio(const float aspectRatio) noexcept {
			m_AspectRatio = aspectRatio;
			UpdateProjectionAndViewProjection(m_FOVY, m_AspectRatio, m_ZNear, m_ZFar);
		}

	private:
		void UpdateViewAndViewProjection() noexcept {
			m_ViewMatrix = glm::mat4_cast(glm::conjugate(m_Orientation)) * glm::translate(glm::mat4(1.0f), -m_Position);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		void UpdateProjectionAndViewProjection(const float fovy, 
											   const float aspectRatio,
											   const float zNear,
											   const float zFar) noexcept {
			m_ProjectionMatrix     = glm::perspective(fovy, aspectRatio, zNear, zFar);
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

	private:
		float m_FOVY;
		float m_AspectRatio;
		float m_ZNear;
		float m_ZFar;

		glm::vec3 m_Position;
		glm::quat m_Orientation;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}
