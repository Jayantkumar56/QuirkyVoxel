

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <optional>
#include <memory>


namespace Mct {

	class VertexArray : public NonCopyable {
	public:
		VertexArray()  noexcept;
		~VertexArray() noexcept;

		void Bind()   const noexcept;
		void Unbind() const noexcept;

		VertexArray(VertexArray&& other)            noexcept;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) noexcept;
		void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer, const bool instanced = false);

		std::shared_ptr<IndexBuffer>&            GetIndexBuffer()   noexcept { return m_IndexBuffer;   }
		std::span<std::shared_ptr<VertexBuffer>> GetVertexBuffers() noexcept { return m_VertexBuffers; }

	protected:
		uint32_t m_Index{};
		uint32_t m_RendererId{};

		std::shared_ptr<IndexBuffer>               m_IndexBuffer;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	};

}
