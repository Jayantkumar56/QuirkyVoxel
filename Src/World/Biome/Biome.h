

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BiomeDataManager.h"


namespace Mct {

	class Biome {
	public:
		constexpr Biome(BiomeId id) noexcept : m_Id(id) {}

		[[nodiscard]] constexpr BiomeId GetId() const noexcept { return m_Id; }



	private:
		BiomeId m_Id;
	};

}
