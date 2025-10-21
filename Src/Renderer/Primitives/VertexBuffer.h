

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BufferLayout.h"
#include "Common/NonCopyable.h"

#include <type_traits>


namespace Mct {

	class VertexBuffer : public NonCopyable {
	public:
		VertexBuffer()                    noexcept;
		VertexBuffer(const uint32_t size) noexcept;
		~VertexBuffer()                   noexcept;

		VertexBuffer(VertexBuffer&& other)            noexcept( std::is_nothrow_move_constructible_v<BufferLayout> );
		VertexBuffer& operator=(VertexBuffer&& other) noexcept( std::is_nothrow_move_assignable_v<BufferLayout>    );

		void UploadData(const void* data, const uint32_t size, const uint32_t offset = 0) noexcept;

		void Bind()   const noexcept;
		void Unbind() const noexcept;

		[[nodiscard]] const BufferLayout& GetLayout() const noexcept { return m_Layout; }
		void SetLayout(BufferLayout layout) { m_Layout = std::move(layout); }

		[[nodiscard]] uint32_t GetSize()  const noexcept { return m_Size; }
		[[nodiscard]] uint32_t GetCount() const noexcept { return m_Size / m_Layout.GetStride(); }

	private:
		uint32_t     m_Size{};
		uint32_t     m_RendererId{};
		BufferLayout m_Layout{};
	};

}
