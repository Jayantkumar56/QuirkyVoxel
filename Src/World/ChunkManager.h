

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Chunk/Chunk.h"
#include "Chunk/ChunkNeighbor.h"
#include "Utils/TaskProcessorPool.h"
#include "Utils/ThreadSafeVector.h"
#include "TerrainGeneration/TerrainGenerator.h"
#include "Chunk/ChunkMeshGenerator.h"

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <optional>


namespace Mct {

	class ChunkManager {
	public:
		static const Subchunk& GetEmptySubchunk() noexcept { return s_EmptySubchunk; }

	public:
		ChunkManager(TerrainGenerator generator);

		void Update(glm::vec3 playerPos);

		auto& GetChunks() { return m_LoadedChunks; }
		std::shared_ptr<Chunk> GetChunk(ChunkCoord pos);

	private:
		std::optional<ChunkMeshInput> GetChunkMeshInputData(ChunkCoord pos);

	private:
		static inline std::array<Block, WorldConst::SubchunkBlockCount> s_EmptySubchunkData;
		static inline Subchunk s_EmptySubchunk{ glm::vec3(0.0f), ChunkManager::s_EmptySubchunkData.data() };

	private:
		class TerrainGenFunctor {	
			using ResultVector_T = ThreadSafeVector<std::shared_ptr<Chunk>>;

		public:
			TerrainGenFunctor(TerrainGenerator terrainGenerator, ResultVector_T* resultVector) :
					m_TerrainGenerator ( terrainGenerator ),
					m_ResultVector     ( resultVector     )
			{}

			void operator()(ChunkCoord taskChunkCoord) {
				auto newChunk = std::make_shared<Chunk>(taskChunkCoord);
				m_TerrainGenerator.GenerateFor(*newChunk);
				m_ResultVector->Push(std::move(newChunk));
			}

		private:
			TerrainGenerator m_TerrainGenerator;
			ResultVector_T*  m_ResultVector;        // Points to the result owned by ChunkManager.
		};

		struct ChunkMeshGenResult {
			ChunkCoord                  ChunkCoord;
			std::unique_ptr<ChunkMesh>  ChunkMesh;
		};

		class ChunkMeshGenFunctor {
		public:
			ChunkMeshGenFunctor(ThreadSafeVector<ChunkMeshGenResult>* resultVector) : 
					m_ResultVector(resultVector)
			{}

			void operator()(ChunkMeshInput meshInput) {
				std::shared_ptr<Chunk>&    chunk     = meshInput.GetMainChunk();
				std::unique_ptr<ChunkMesh> chunkMesh = std::make_unique<ChunkMesh>();

				ChunkMeshGenerator::GenerateChunkMeshes(*chunkMesh, meshInput);
				m_ResultVector->Push(ChunkMeshGenResult{chunk->GetCoord(), std::move(chunkMesh)});
			}

		private:
			// Points to the result owned by ChunkManager.
			ThreadSafeVector<ChunkMeshGenResult>* m_ResultVector;
		};

	private:
		std::unordered_set<ChunkCoord>                   m_ChunksInTerrainGeneration;
		ThreadSafeVector<std::shared_ptr<Chunk>>         m_TerrainGeneratorResults;
		TaskProcessorPool<ChunkCoord, TerrainGenFunctor> m_TerrainGeneratorPool;

		std::unordered_set<ChunkCoord>                         m_ChunksInMeshGeneration;
		ThreadSafeVector<ChunkMeshGenResult>                   m_ChunkMeshGenResults;
		TaskProcessorPool<ChunkMeshInput, ChunkMeshGenFunctor> m_ChunkMeshGenPool;

		// All chunks in memory (Inner renderable + Outer non renderable zones)
		std::unordered_map<ChunkCoord, std::shared_ptr<Chunk>> m_LoadedChunks;
	};

}
