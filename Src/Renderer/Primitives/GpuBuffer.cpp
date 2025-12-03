

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GpuBuffer.h"
#include "Utils/Assert.h"

#include <glad/glad.h>


namespace Mct {

	GpuBuffer::GpuBuffer() noexcept {
        glCreateBuffers(1, &m_RendererId);
        m_SizeInBytes = 0;
    }

    GpuBuffer::~GpuBuffer() {
        if (m_RendererId) {
            glDeleteBuffers(1, &m_RendererId);
        }
    }

    GpuBuffer::GpuBuffer(GpuBuffer&& other) noexcept : 
            m_RendererId  ( std::exchange(other.m_RendererId,  0) ),
            m_SizeInBytes ( std::exchange(other.m_SizeInBytes, 0) )
    {}

    GpuBuffer& GpuBuffer::operator=(GpuBuffer && other) noexcept {
        if (this != &other) {
            if (m_RendererId) {
                glDeleteBuffers(1, &m_RendererId);
            }

            m_RendererId  = std::exchange(other.m_RendererId,  0);
            m_SizeInBytes = std::exchange(other.m_SizeInBytes, 0);
        }

        return *this;
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

}
