

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "VertexArray.h"

#include <glad/glad.h>


namespace Mct {

	VertexArray::VertexArray() noexcept {
		glCreateVertexArrays(1, &m_RendererId);
	}

	VertexArray::~VertexArray() noexcept {
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void VertexArray::Bind() const noexcept {
		glBindVertexArray(m_RendererId);
	}

	void VertexArray::Unbind() const noexcept {
		glBindVertexArray(0);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept :
			m_Index         ( other.m_Index                    ),
			m_RendererId    ( other.m_RendererId               ),
			m_IndexBuffer   ( std::move(other.m_IndexBuffer)   ),
			m_VertexBuffers ( std::move(other.m_VertexBuffers) )
	{
		other.m_Index      = 0;
		other.m_RendererId = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
		if (this != &other) {
			if (m_RendererId > 0) {
				glDeleteVertexArrays(1, &m_RendererId);
			}

			m_Index         = other.m_Index;
			m_RendererId    = other.m_RendererId;
			m_IndexBuffer   = std::move(other.m_IndexBuffer);
			m_VertexBuffers = std::move(other.m_VertexBuffers);

			other.m_Index = 0;
			other.m_RendererId = 0;
		}

		return *this;
	}

	void VertexArray::SetIndexBuffer(IndexBuffer&& indexBuffer) noexcept {
		glBindVertexArray(m_RendererId);
		indexBuffer.Bind();
		m_IndexBuffer = std::move(indexBuffer);
	}

	void VertexArray::AddVertexBuffer(VertexBuffer&& vertexBuffer, const bool instanced) {
		MCT_ASSERT(vertexBuffer.GetLayout().GetElements().size() && "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererId);
		vertexBuffer.Bind();

		const auto& layout = vertexBuffer.GetLayout();
		uint64_t offset = 0;

		for (const auto& element : layout) {
			if (element.IsIntType()) {
				glEnableVertexAttribArray(m_Index);
				glVertexAttribIPointer(m_Index, GetComponentCount(element.Type), GL_INT, layout.GetStride(), (const void*)offset);

				if (instanced)
					glVertexAttribDivisor(m_Index, 1);

				++m_Index;
			}
			else if (element.Type == ShaderDataType::Mat4) {
				uint64_t elementOffset = offset;
				for (int i = 0; i < 4; ++i, elementOffset += static_cast<uint64_t>(4 * 4)) {
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 4, GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)elementOffset);

					if (instanced)
						glVertexAttribDivisor(m_Index, 1);

					++m_Index;
				}
			}
			else if (element.Type == ShaderDataType::Mat3) {
				uint64_t elementOffset = offset;
				for (int i = 0; i < 3; ++i, elementOffset += static_cast<uint64_t>(4 * 3)) {
					glEnableVertexAttribArray(m_Index);
					glVertexAttribPointer(m_Index, 3, GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)elementOffset);

					if (instanced)
						glVertexAttribDivisor(m_Index, 1);

					++m_Index;
				}
			}
			else {
				glEnableVertexAttribArray(m_Index);
				glVertexAttribPointer(m_Index, GetComponentCount(element.Type), GL_FLOAT, GL_FALSE, layout.GetStride(), (const void*)offset);

				if (instanced)
					glVertexAttribDivisor(m_Index, 1);

				++m_Index;
			}

			offset += ShaderDataTypeSize(element.Type);
		}

		m_VertexBuffers.push_back(std::move(vertexBuffer));
	}

}
