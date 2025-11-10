

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GpuBuffer.h"
#include "Common/Assert.h"

#include <glad/glad.h>


namespace Mct {

	GpuBuffer::GpuBuffer() noexcept {
        glCreateBuffers(1, &m_RendererId);
        m_SizeInBytes = 0;
    }

    GpuBuffer::~GpuBuffer() {
        if (m_RendererId != 0)
            glDeleteBuffers(1, &m_RendererId);
    }

    void GpuBuffer::Allocate(const size_t size, const void* data, const uint32_t storageFlags) noexcept {
        glNamedBufferStorage(m_RendererId, size, data, storageFlags);
        m_SizeInBytes = size;
    }

    void GpuBuffer::Upload(const void* data, size_t size, size_t offset) const noexcept {
        MCT_ASSERT(m_RendererId != 0 && "GpuBuffer have no allocated buffer on GPU.");
        MCT_ASSERT(offset + size <= m_SizeInBytes && "GpuBuffer update is out of bounds!");
        glNamedBufferSubData(m_RendererId, offset, size, data);
    }

    void GpuBuffer::DeleteBuffer() const noexcept {
        if (m_RendererId != 0)
            glDeleteBuffers(1, &m_RendererId);
    }

}
