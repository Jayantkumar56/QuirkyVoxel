

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "VertexBuffer.h"

#include <glad/glad.h>


namespace Mct {

	VertexBuffer::VertexBuffer(const uint32_t size) noexcept {
		Allocate(size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept(std::is_nothrow_move_constructible_v<BufferLayout>) :
			GpuBuffer ( std::move(other)          ),
			m_Layout  ( std::move(other.m_Layout) )
	{
		other.m_RendererId = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept(std::is_nothrow_move_assignable_v<BufferLayout>) {
		if (this != &other) {
			GpuBuffer::operator=(std::move(other));
			m_Layout = std::move(other.m_Layout);
		}

		return *this;
	}

	void VertexBuffer::Bind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void VertexBuffer::Unbind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
