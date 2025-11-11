

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


namespace Mct {

    struct NonCopyable {
        constexpr NonCopyable()  noexcept = default;
        constexpr ~NonCopyable()          = default;

        // Deleted copy members
        constexpr NonCopyable(const NonCopyable&)            noexcept = delete;
        constexpr NonCopyable& operator=(const NonCopyable&) noexcept = delete;

        constexpr NonCopyable(NonCopyable&&)            noexcept = default;
        constexpr NonCopyable& operator=(NonCopyable&&) noexcept = default;
    };

}
