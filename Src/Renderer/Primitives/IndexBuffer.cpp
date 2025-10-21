

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "IndexBuffer.h"

#include <glad/glad.h>


namespace Mct {

	IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count) noexcept {
		m_Count = count;

		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() noexcept {
		glDeleteBuffers(1, &m_RendererId);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept :
			m_Count      ( other.m_Count      ),
			m_RendererId ( other.m_RendererId)
	{
		other.m_Count      = 0;
		other.m_RendererId = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
		if (this != &other) {
			if (m_RendererId > 0) {
				glDeleteBuffers(1, &m_RendererId);
			}

			m_Count      = other.m_Count;
			m_RendererId = other.m_RendererId;

			other.m_Count      = 0;
			other.m_RendererId = 0;
		}

		return *this;
	}

	void IndexBuffer::Bind() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void IndexBuffer::Unbind() const noexcept {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
