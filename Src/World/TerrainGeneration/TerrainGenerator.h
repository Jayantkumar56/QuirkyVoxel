

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/WorldSettings.h"
#include "SuperFlatTerrainGen.h"

#include <memory>
#include <variant>


namespace Mct {

	class Chunk;

	class TerrainGenerator {
	public:
		template<typename T>
		static TerrainGenerator Create() {
			TerrainGenerator terrainGenerator;
			terrainGenerator.m_Generator.emplace<T>();
			return terrainGenerator;
		}

		virtual void GenerateFor(Chunk& chunk) {
			std::visit([&chunk](auto&& activeGenerator) {
				using T = std::decay_t<decltype(activeGenerator)>;

				if constexpr (!std::is_same_v<T, std::monostate>) {
					activeGenerator.GenerateFor(chunk);
				}
			}, m_Generator);
		}

	private:
		std::variant<
			std::monostate, 
			SuperFlatTerrainGen
		> m_Generator;
	};

}
