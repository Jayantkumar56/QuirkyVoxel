

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "IndexBuffer.h"

#include <glad/glad.h>


namespace Mct {

	IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count) noexcept {
		m_Count = count;
		Allocate(count * sizeof(uint32_t), indices, GL_DYNAMIC_STORAGE_BIT);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept :
		    GpuBuffer ( std::move(other) ),
			m_Count   ( other.m_Count    )
	{
		other.m_Count = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
		if (this != &other) {
			GpuBuffer::operator=(std::move(other));
			m_Count        = other.m_Count;
			other.m_Count  = 0;
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
