

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "GpuBuffer.h"


namespace Mct {

    class ShaderStorageBuffer : public GpuBuffer {
    public:
        ShaderStorageBuffer() noexcept = default;
        ShaderStorageBuffer(size_t size) noexcept;

        void Bind(uint32_t bindingPoint) const noexcept;
    };

}
