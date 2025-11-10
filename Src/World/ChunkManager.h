

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Chunk/Chunk.h"
#include "Chunk/ChunkNeighbor.h"

#include <glm/glm.hpp>

#include <memory>
#include <array>
#include <set>
#include <unordered_map>
#include <optional>


namespace Mct {

	class TerrainGenerator;

	class ChunkManager {
	public:
		static const Subchunk& GetEmptySubchunk() noexcept { return s_EmptySubchunk; }

	public:
		ChunkManager(TerrainGenerator& generator) :
				m_TerrainGenerator(generator)
		{}

		auto& GetChunks() { return m_LoadedChunks; }

		std::shared_ptr<Chunk>        GetChunk(ChunkCoord pos);
		std::optional<ChunkMeshInput> GetChunkMeshInputData(ChunkCoord pos);

		std::set<ChunkCoord>& GetRemeshQueueToProcess() noexcept { return m_MeshBuildQueue; }

		void Update(glm::vec3 playerPos);
		void UpdateVisibleChunks(glm::vec3 playerPos);

	private:
		static inline std::array<Block, WorldConst::SubchunkBlockCount> s_EmptySubchunkData;
		static inline Subchunk s_EmptySubchunk{ glm::vec3(0.0f), ChunkManager::s_EmptySubchunkData.data() };

	private:
		TerrainGenerator& m_TerrainGenerator;

		// Chunks that are ready to be meshed (Inner zone + modified chunks)
		std::set<ChunkCoord> m_MeshBuildQueue;

		// All chunks in memory (Inner renderable + Outer non renderable zones)
		std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> m_LoadedChunks;
	};

}
