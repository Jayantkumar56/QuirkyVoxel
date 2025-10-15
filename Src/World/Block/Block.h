

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockType.h"


namespace Mct {

	class Block {
	public:
		// Initailise with default air block
		constexpr Block() noexcept : m_Type(BlockType::Air) {}

		constexpr Block(BlockType type) noexcept : m_Type(type) {}

		[[nodiscard]] constexpr BlockType GetType() const noexcept { return m_Type; }

	private:
		BlockType m_Type;
	};

}
