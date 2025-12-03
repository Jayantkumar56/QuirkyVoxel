

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Renderer/Primitives/Texture2D.h"

#include <glm/glm.hpp>


namespace Mct {

	class WorldTime;

	struct SkyBlendWeights {
		float Sunrise  = 0.0f;
		float Day      = 0.0f;
		float Night    = 0.0f;
		float Midnight = 0.0f;
	};

	class Sky {
	public:
		Sky();

		void Update(const WorldTime& worldTime);

		[[nodiscard]] SkyBlendWeights GetGradientWeights() const noexcept { return m_Weights; }

		[[nodiscard]] const Texture2D& GetSunriseGradient()  const noexcept { return m_SunriseGradient;  }
		[[nodiscard]] const Texture2D& GetDayGradient()      const noexcept { return m_DayGradient;      }
		[[nodiscard]] const Texture2D& GetNightGradient()    const noexcept { return m_NightGradient;    }
		[[nodiscard]] const Texture2D& GetMidnightGradient() const noexcept { return m_MidnightGradient; }

		[[nodiscard]] const Texture2D& GetMoonTexture() const noexcept { return m_MoonTexture; }

		[[nodiscard]] glm::vec3 GetSunDirection()  const noexcept { return m_SunDirection;  }
		[[nodiscard]] glm::vec3 GetMoonDirection() const noexcept { return -m_SunDirection; }

		[[nodiscard]] float GetSunRadius()      const noexcept { return m_SunRadius;      }
		[[nodiscard]] float GetMoonRadius()     const noexcept { return m_MoonRadius;     }
		[[nodiscard]] float GetSunVisibility()  const noexcept { return m_SunVisibility;  }
		[[nodiscard]] float GetMoonVisibility() const noexcept { return m_MoonVisibility; }

	private:
		void UpdateSunDirection(const WorldTime& worldTime) noexcept;

		[[nodiscard]] TextureProperties GetGradientTextureProperties() const noexcept;
		[[nodiscard]] TextureProperties GetMoonTextureProperties()     const noexcept;

		[[nodiscard]] SkyBlendWeights CalculateWeights(float hour) const noexcept;

		void UpdateSunMoonVisibility(float hour) noexcept;

	private:
		Texture2D m_SunriseGradient;
		Texture2D m_DayGradient;
		Texture2D m_NightGradient;
		Texture2D m_MidnightGradient;

		Texture2D m_MoonTexture;

		SkyBlendWeights m_Weights;

		float m_SunVisibility;
		float m_MoonVisibility;

		float m_SunRadius;
		float m_MoonRadius;

		glm::vec3 m_SunDirection;
	};

}
