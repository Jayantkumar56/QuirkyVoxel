

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Types.h"

#include <cinttypes>


namespace Mct {

    using BufferLayoutId = uint32_t;

    // This is just a CPU-side handle. It owns NO GPU resources.
    class VertexBufferHandle {
    public:
        constexpr VertexBufferHandle(const size_t         offset, 
                                     const size_t         size,
                                     const BufferLayoutId layout) noexcept : 
                m_Offset ( offset ), 
                m_Size   ( size   ), 
                m_Layout ( layout ) 
        {}

        // Get the offset (into the massive VBO).
        [[nodiscard]] constexpr size_t         GetOffset()   const noexcept { return m_Offset; }
        [[nodiscard]] constexpr size_t         GetSize()     const noexcept { return m_Size;   }
        [[nodiscard]] constexpr BufferLayoutId GetLayoutId() const noexcept { return m_Layout; }

    private:
        size_t          m_Offset;
        size_t          m_Size;
        BufferLayoutId  m_Layout;
        RenderPrimitive PrimitiveType = RenderPrimitive::Triangles;
    };

    // This is just a CPU-side handle. It owns NO GPU resources.
    class IndexBufferHandle {
    public:
        constexpr IndexBufferHandle(const size_t    offset, 
                                    const size_t    size,
                                    const IndexType type) noexcept : 
                m_Offset   ( offset ), 
                m_Size     ( size   ), 
                m_IndexType( type   ) 
        {}

        // Get the offset (into the massive IBO).
        [[nodiscard]] constexpr size_t    GetOffset()  const noexcept { return m_Offset;    }
        [[nodiscard]] constexpr size_t    GetSize()    const noexcept { return m_Size;      }
        [[nodiscard]] constexpr IndexType GetType()    const noexcept { return m_IndexType; }

        // Get the byte offset as the (void*) pointer OpenGL expects.
        [[nodiscard]] constexpr const void* GetGlOffset() const noexcept { return (const void*)m_Offset; }

    private:
        size_t    m_Offset;
        size_t    m_Size;
        IndexType m_IndexType;
    };

}
