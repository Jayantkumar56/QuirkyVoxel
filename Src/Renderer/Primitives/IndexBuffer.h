

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/NonCopyable.h"

#include <cstdint>


namespace Mct {

	class IndexBuffer : public NonCopyable {
	public:
		IndexBuffer(const uint32_t* indices, const uint32_t count) noexcept;
		~IndexBuffer() noexcept;

		IndexBuffer(IndexBuffer&& other)            noexcept;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;

		void Bind()   const noexcept;
		void Unbind() const noexcept;

	private:
		uint32_t m_Count{};
		uint32_t m_RendererId{};
	};

}
