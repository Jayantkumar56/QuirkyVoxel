

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BufferLayout.h"
#include "GpuBuffer.h"

#include <type_traits>


namespace Mct {

	class VertexBuffer : public GpuBuffer {
	public:
		VertexBuffer() noexcept = default;
		VertexBuffer(size_t size) noexcept;

		VertexBuffer(VertexBuffer&& other)            noexcept( std::is_nothrow_move_constructible_v<BufferLayout> );
		VertexBuffer& operator=(VertexBuffer&& other) noexcept( std::is_nothrow_move_assignable_v<BufferLayout>    );

		void Bind()   const noexcept;
		void Unbind() const noexcept;

		[[nodiscard]] const BufferLayout& GetLayout() const noexcept { return m_Layout; }
		void SetLayout(BufferLayout layout) { m_Layout = std::move(layout); }

		[[nodiscard]] uint32_t GetCount() const noexcept { 
			return static_cast<uint32_t>(m_SizeInBytes / m_Layout.GetStride()); 
		}

	private:
		BufferLayout m_Layout{};
	};

}
