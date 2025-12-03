

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace Mct {

    struct TimeData {
        uint8_t  Seconds;
        uint8_t  Minutes;
        uint8_t  Hours;
        uint32_t Days;

        constexpr auto operator <=>(const TimeData&) const noexcept = default;
    };

    class WorldTime {
        // 1 game minute = 1 real second
        static constexpr float c_GameSecondsPerRealSecond = 60.0f;
        static constexpr float c_SecondsPerMinutes = 60.0f * 60.0f;
        static constexpr float c_SecondsPerHour    = 60.0f * 60.0f;
        static constexpr float c_SecondsPerDay     = c_SecondsPerHour * 24.0f;

    public:
        [[nodiscard]] static constexpr float RealToGameSeconds(float realSeconds) noexcept {
            return realSeconds * c_GameSecondsPerRealSecond;
        }

        [[nodiscard]] static constexpr float GetSecondsInDay() noexcept {
            return c_SecondsPerDay;
        }

        [[nodiscard]] static constexpr float GetSecondsInHour() noexcept {
            return c_SecondsPerHour;
        }

    public:
        constexpr WorldTime(bool pause = false) noexcept {
            // Start time from 6AM.
            const float dayStartTime = 6 * c_SecondsPerHour;

            m_IsPause     = true;
            m_CurrentTime = dayStartTime;
            m_TimeScale   = 1.0f;
            m_DayCount    = 0;
        }

        void Update(float deltaTime) noexcept {
            if (m_IsPause)
                return;

            m_CurrentTime += RealToGameSeconds(m_TimeScale * deltaTime);

            while (m_CurrentTime >= c_SecondsPerDay) {
                m_CurrentTime -= c_SecondsPerDay;
                ++m_DayCount;
            }
        }

        [[nodiscard]] constexpr TimeData GetTime() const noexcept {
            const auto totalSeconds = static_cast<uint32_t>(m_CurrentTime);

            return TimeData{
                .Seconds { static_cast<uint8_t>(totalSeconds % 60)          },
                .Minutes { static_cast<uint8_t>((totalSeconds / 60) % 60)   },
                .Hours   { static_cast<uint8_t>((totalSeconds / 3600) % 24) },
                .Days    { m_DayCount                                       }
            };
        }

        // Returns Current time of the day in seconds.
        [[nodiscard]] constexpr float GetCurrentTimeRaw() const noexcept {
            return m_CurrentTime;
        }

        [[nodiscard]] constexpr float GetTimeScale() const noexcept {
            return m_TimeScale;
        }

        constexpr void SetTimeScale(float scale) noexcept {
            m_TimeScale = scale;
        }

        constexpr void SetTime(const TimeData& time) {
            m_DayCount = time.Days;

            const float timeInSeconds = static_cast<float>(time.Seconds)                       + 
                                        static_cast<float>(time.Minutes) * c_SecondsPerMinutes +
                                        static_cast<float>(time.Hours)   * c_SecondsPerHour;

            const uint32_t dayCoundInTime = static_cast<uint32_t>(timeInSeconds / c_SecondsPerDay);

            m_DayCount   += dayCoundInTime;
            m_CurrentTime = timeInSeconds - (static_cast<float>(dayCoundInTime) * c_SecondsPerDay);
        }

        constexpr void Pause() noexcept { m_IsPause = true;  }
        constexpr void Play()  noexcept { m_IsPause = false; }

    private:
        bool m_IsPause;

        float m_CurrentTime;
        float m_TimeScale;

        uint32_t m_DayCount;
    };

}
