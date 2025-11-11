

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"

#include <utility>


namespace Mct {


    class GpuBuffer : public NonCopyable {
    public:
        GpuBuffer() noexcept;
        ~GpuBuffer();

        GpuBuffer(GpuBuffer&& other) noexcept : 
                m_RendererId  ( other.m_RendererId  ),
                m_SizeInBytes ( other.m_SizeInBytes )
        {
            other.m_RendererId  = 0;
            other.m_SizeInBytes = 0;
        }

        GpuBuffer& operator=(GpuBuffer&& other) noexcept {
            if (this != &other) {
                DeleteBuffer();

                m_RendererId  = other.m_RendererId;
                m_SizeInBytes = other.m_SizeInBytes;

                other.m_RendererId  = 0;
                other.m_SizeInBytes = 0;
            }
            return *this;
        }

        void Allocate(size_t size, const void* data, uint32_t storageFlags) noexcept;
        void Upload(const void* data, size_t size, size_t offset)     const noexcept;

        [[nodiscard]] uint32_t GetHandle() const noexcept { return m_RendererId;  }
        [[nodiscard]] size_t   GetSize()   const noexcept { return m_SizeInBytes; }

    private:
        void DeleteBuffer() const noexcept;

    protected:
        uint32_t m_RendererId;
        size_t   m_SizeInBytes;
    };

}
