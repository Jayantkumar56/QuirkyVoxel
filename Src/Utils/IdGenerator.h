

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <limits>
#include <type_traits>


namespace Mct {

    template<typename T>
    requires std::is_integral_v<T>
    constexpr T InvalidId = std::numeric_limits<T>::max();

    template<typename T>
    requires std::is_integral_v<T>
    class LinearIdGenerator {
    public:
        constexpr LinearIdGenerator(T startingId) noexcept :
                m_NextId(startingId)
        {}

        [[nodiscard]] constexpr T GetNext() noexcept {
            return m_NextId++;
        }

    private:
        T m_NextId;
    };

    inline constexpr size_t InvalidTypeId = InvalidId<size_t>;

    class TypeIdGenerator {
    public:
        template<typename T>
        static size_t Get() noexcept {
            static const size_t id = s_NextId++;
            return id;
        }

    private:
        static inline size_t s_NextId = 0;
    };

}
