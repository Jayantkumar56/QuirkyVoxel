

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/Assert.h"


namespace Mct {

    // floor-div that works for negatives the Minecraft way.
    // Example: FloorDiv(-1, 16) == -1
    [[nodiscard]] constexpr int FloorDiv(const int a, const int b) noexcept {
        MCT_ASSERT(b > 0 && "FloorDiv expects a positive divisor");
        const int q = a / b;
        const int r = a % b;
        return (r && ((r > 0) != (b > 0))) ? (q - 1) : q;
    }

    [[nodiscard]] constexpr float InverseLerp(float start, float end, float value) noexcept {
        return (value - start) / (end - start);
    }

}
