

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "GpuBuffer.h"

#include <glad/glad.h>


namespace Mct {

    template<typename T>
    class IndirectBuffer : public GpuBuffer {
        static constexpr size_t c_ElementSize = sizeof(T);

    public:
        IndirectBuffer(size_t maxElementCount) noexcept :
            m_MaxElementCount(maxElementCount)
        {
            const size_t totalByteSize = m_MaxElementCount * c_ElementSize;
            Allocate(totalByteSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
        }

        IndirectBuffer(IndirectBuffer&& other) noexcept :
                GpuBuffer         ( std::move(other)        ),
                m_MaxElementCount ( other.m_MaxElementCount )
        {
            other.m_MaxElementCount = 0;
        }

        IndirectBuffer& operator=(IndirectBuffer&& other) noexcept {
            if (this != &other) {
                GpuBuffer::operator=(std::move(other));
                m_MaxElementCount = other.m_MaxElementCount;
                other.m_MaxElementCount = 0;
            }
            return *this;
        }

        void Bind() const noexcept {
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_RendererId);
        }

        void Unbind() const noexcept {
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
        }

        void UploadData(const T* data, size_t elementCount, size_t elementOffset = 0) const noexcept {
            Upload(
                data,
                elementCount  * c_ElementSize,   // Calculate byte size
                elementOffset * c_ElementSize    // Calculate byte offset
            );
        }

        size_t GetMaxElementCount() const noexcept { return m_MaxElementCount; }

    private:
        size_t m_MaxElementCount;
    };

}
