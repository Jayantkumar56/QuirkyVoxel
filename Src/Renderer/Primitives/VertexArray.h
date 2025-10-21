

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <optional>


namespace Mct {

	class VertexArray : public NonCopyable {
	public:
		VertexArray()  noexcept;
		~VertexArray() noexcept;

		void Bind()   const noexcept;
		void Unbind() const noexcept;

		VertexArray(VertexArray&& other)            noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void SetIndexBuffer(IndexBuffer&& indexBuffer) noexcept;
		void AddVertexBuffer(VertexBuffer&& vertexBuffer, const bool instanced = false);

		const std::optional<IndexBuffer>& GetIndexBuffer()   const noexcept { return m_IndexBuffer;   }
		std::span<const VertexBuffer>     GetVertexBuffers() const noexcept { return m_VertexBuffers; }

	protected:
		uint32_t m_Index{};
		uint32_t m_RendererId{};

		std::optional<IndexBuffer> m_IndexBuffer;
		std::vector<VertexBuffer>  m_VertexBuffers;
	};

}
