

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <type_traits>


namespace Mct {

    template<typename T>
    requires std::is_integral_v<T>
    class LinearIdGenerator {
    public:
        constexpr LinearIdGenerator(const int32_t startingId = 0) noexcept : 
                m_NextId(startingId)
        {}

        [[nodiscard]] constexpr int32_t GetNext() noexcept { 
            return m_NextId++;
        }

    private:
        T m_NextId;
    };

}
