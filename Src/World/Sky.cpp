

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Sky.h"
#include "Utils/Math.h"
#include "WorldTime.h"


namespace Mct {

	Sky::Sky() :
			m_DayGradient      ("Assets/Textures/sky_day.png",      GetGradientTextureProperties()),
			m_SunriseGradient  ("Assets/Textures/sky_sunrise.png",  GetGradientTextureProperties()),
			m_NightGradient    ("Assets/Textures/sky_night.png",    GetGradientTextureProperties()),
			m_MidnightGradient ("Assets/Textures/sky_midnight.png", GetGradientTextureProperties()),
			m_MoonTexture      ("Assets/Textures/moon.png",         GetMoonTextureProperties()    )
	{
		m_Weights = { 0.0f, 0.0f, 0.0f, 0.0f };

		m_SunRadius  = glm::radians(2.0f);
		m_MoonRadius = glm::radians(2.5f);
	}

	void Sky::Update(const WorldTime& worldTime) {
		UpdateSunDirection(worldTime);

		const float tsec  = worldTime.GetCurrentTimeRaw();
		const float hourD = fmod(tsec / WorldTime::GetSecondsInHour(), 24.0f);
		const float hour  = hourD < 0.0f ? hourD + 24.0f : hourD;

		UpdateSunMoonVisibility(hour);
		m_Weights = CalculateWeights(hour);
	}

	void Sky::UpdateSunDirection(const WorldTime& worldTime) noexcept {
		const float time  = worldTime.GetCurrentTimeRaw() / WorldTime::GetSecondsInDay();
		const float angle = (time * glm::two_pi<float>());

		const glm::vec3 sunDir{
			std::sin(angle),
			-std::cos(angle),
			0.0f
		};

		m_SunDirection = sunDir;
	}

	TextureProperties Sky::GetGradientTextureProperties() const noexcept {
		TextureProperties props;
		props.WrapS = TextureWrap::MirroredRepeat;
		props.WrapT = TextureWrap::MirroredRepeat;
		return props;
	}

	TextureProperties Sky::GetMoonTextureProperties() const noexcept {
		TextureProperties props;
		props.MipLevels = 3;
		return props;
	}

	SkyBlendWeights Sky::CalculateWeights(float hour) const noexcept {
		constexpr float midnightEnd   = 0.0f;
		constexpr float nightStart    = 3.0f;
		constexpr float sunriseStart  = 6.0f;
		constexpr float dayStart      = 8.0f;
		constexpr float sunsetStart   = 17.0f;
		constexpr float eveNightStart = 19.0f;
		constexpr float midnightStart = 24.0f;

		SkyBlendWeights w{};

		if (hour <= sunriseStart) {
			w.Night    = InverseLerp(midnightEnd, sunriseStart, hour);
			w.Midnight = 1.0f - w.Night;
		}
		else if (hour <= dayStart) {
			constexpr float sunriseMid = sunriseStart + 0.5f * (dayStart - sunriseStart);

			if (hour <= sunriseMid) {
				w.Sunrise = InverseLerp(sunriseStart, sunriseMid, hour);
				w.Night   = 1.0f - w.Sunrise;
			}
			else {
				w.Day     = InverseLerp(sunriseMid, dayStart, hour);
				w.Sunrise = 1.0f - w.Day;
			}
		}
		else if (hour <= sunsetStart) {
			w.Day = 1.0f;
		}
		else if (hour <= eveNightStart) {
			constexpr float sunsetMid = sunsetStart + 0.5f * (eveNightStart - sunsetStart);

			// Evaluating sunrise weight here since using same texture for both sunrise and sunset.
			if (hour <= sunsetMid) {
				w.Sunrise = InverseLerp(sunsetStart, sunsetMid, hour);
				w.Day     = 1.0f - w.Sunrise;
			}
			else {
				w.Night   = InverseLerp(sunsetMid, eveNightStart, hour);
				w.Sunrise = 1.0f - w.Night;
			}
		}
		else {
			w.Midnight = InverseLerp(eveNightStart, midnightStart, hour);
			w.Night    = 1.0f - w.Midnight;
		}

		return w;
	}

	void Sky::UpdateSunMoonVisibility(float hour) noexcept {
		m_SunVisibility  = 0.0f;
		m_MoonVisibility = 0.0f;

		m_SunVisibility  = glm::smoothstep(5.0f, 7.0f, hour) * (1 - glm::smoothstep(18.0f, 19.0f, hour));
		m_MoonVisibility = (1.0f - glm::smoothstep(5.0f, 7.0f, hour)) + glm::smoothstep(16.0f, 18.0f, hour);
	}

}
