

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


namespace Mct {

    struct NonMovable {
        constexpr NonMovable()  noexcept = default;
        constexpr ~NonMovable()          = default;

        constexpr NonMovable(const NonMovable&)            noexcept = default;
        constexpr NonMovable& operator=(const NonMovable&) noexcept = default;

        // Deleted move members
        constexpr NonMovable(NonMovable&&)            noexcept = delete;
        constexpr NonMovable& operator=(NonMovable&&) noexcept = delete;
    };

}
