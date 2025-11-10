

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "ShaderStorageBuffer.h"

#include <glad/glad.h>


namespace Mct {

    ShaderStorageBuffer::ShaderStorageBuffer(size_t size) noexcept {
        Allocate(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }

    void ShaderStorageBuffer::Bind(uint32_t bindingPoint) const noexcept {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_RendererId);
    }

}
