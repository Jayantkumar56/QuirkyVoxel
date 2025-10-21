

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "VertexBuffer.h"

#include <glad/glad.h>


namespace Mct {

	VertexBuffer::VertexBuffer() noexcept {
		m_Size = 0;
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	VertexBuffer::VertexBuffer(const uint32_t size) noexcept {
		m_Size = size;

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() noexcept {
		glDeleteBuffers(1, &m_RendererId);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept(std::is_nothrow_move_constructible_v<BufferLayout>) :
			m_Size       ( other.m_Size              ),
			m_RendererId ( other.m_RendererId        ),
			m_Layout     ( std::move(other.m_Layout) )
	{
		other.m_Size       = 0;
		other.m_RendererId = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept(std::is_nothrow_move_assignable_v<BufferLayout>) {
		if (this != &other) {
			if (m_RendererId > 0) {
				glDeleteBuffers(1, &m_RendererId);
			}

			m_Size       = other.m_Size;
			m_RendererId = other.m_RendererId;
			m_Layout     = std::move(other.m_Layout);

			other.m_Size       = 0;
			other.m_RendererId = 0;
		}

		return *this;
	}

	void VertexBuffer::UploadData(const void* data, const uint32_t size, const uint32_t offset) noexcept {
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void VertexBuffer::Unbind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
