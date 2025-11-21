

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "ChunkManager.h"
#include "TerrainGeneration/TerrainGenerator.h"


namespace Mct {

    ChunkManager::ChunkManager(TerrainGenerator generator) :
            m_TerrainGenerator     ( std::move(generator) ),
			m_TerrainGeneratorPool ( 6, TerrainGenFunctor(m_TerrainGenerator, &m_TerrainGeneratorResults) ),
			m_ChunkMeshGenPool     ( 6, ChunkMeshGenFunctor(&m_ChunkMeshGenResults)              )
	{}

    void ChunkManager::Update(glm::vec3 playerPos) {
        ProcessPendingResults();

        const ChunkCoord playerChunkPos = ChunkCoord::FromWorldXZ(
            static_cast<int>(std::floor(playerPos.x)),
            static_cast<int>(std::floor(playerPos.z))
        );

        LoadChunksInRange(playerChunkPos);
        UnloadOutOfRangeChunks(playerChunkPos);
	}

    std::shared_ptr<Chunk> ChunkManager::GetChunk(ChunkCoord pos) {
        if (auto it = m_LoadedChunks.find(pos); it != m_LoadedChunks.end())
            return it->second;

        return nullptr;
    }

    void ChunkManager::ProcessPendingResults() {
        // Stores chunks returned by the terrain generator.
        {
            std::vector<std::shared_ptr<Chunk>> results = m_TerrainGeneratorResults.Drain();

            for (auto& result : results) {
                ChunkCoord currChunkCoord = result->GetCoord();
                m_LoadedChunks[currChunkCoord] = std::move(result);
                m_ChunksInTerrainGeneration.erase(currChunkCoord);
            }
        }

        // Stores mesh returned by the mesh generator.
        {
            std::vector<ChunkMeshGenResult> results = m_ChunkMeshGenResults.Drain();

            for (auto& result : results) {
                auto it = m_LoadedChunks.find(result.ChunkCoord);

                if (it != m_LoadedChunks.end()) {
                    it->second->SetMesh(std::move(result.ChunkMesh));
                }

                m_ChunksInMeshGeneration.erase(result.ChunkCoord);
            }
        }
    }

    void ChunkManager::LoadChunksInRange(ChunkCoord playerChunkPos) {
        const int loadDist   = WorldConst::LoadDistance;
        const int renderDist = WorldConst::RenderDistance;

        std::vector<ChunkCoord> chunksForTerrainGen;

        std::vector<ChunkCoord> chunksForMeshGen;
        std::vector<ChunkMeshInput> chunksMeshGenInputs;

        for (int x = playerChunkPos.X - loadDist; x <= playerChunkPos.X + loadDist; ++x) {
            for (int z = playerChunkPos.Z - loadDist; z <= playerChunkPos.Z + loadDist; ++z) {
                ChunkCoord pos = { x, z };

                if (m_LoadedChunks.contains(pos)) {
                    const int distFromPlayerX = std::abs(x - playerChunkPos.X);
                    const int distFromPlayerZ = std::abs(z - playerChunkPos.Z);

                    // Queue for Remesh only when in the render distance.
                    if (distFromPlayerX <= renderDist && distFromPlayerZ <= renderDist) {
                        if (!m_ChunksInMeshGeneration.contains(pos) && m_LoadedChunks[pos]->NeedRemesh()) {
                            std::optional<ChunkMeshInput> meshInput = GetChunkMeshInputData(pos);

                            if (meshInput) {
                                chunksMeshGenInputs.emplace_back(std::move(*meshInput));
                                chunksForMeshGen.emplace_back(pos.X, pos.Z);
                            }
                        }
                    }
                }
                else if (!m_ChunksInTerrainGeneration.contains(pos)) {   // If the chunk doesn't exist, add it.
                    chunksForTerrainGen.emplace_back(pos.X, pos.Z);
                }
            }
        }

        m_ChunksInTerrainGeneration.insert(chunksForTerrainGen.begin(), chunksForTerrainGen.end());
        m_TerrainGeneratorPool.SubmitBatch(chunksForTerrainGen);

        m_ChunksInMeshGeneration.insert(chunksForMeshGen.begin(), chunksForMeshGen.end());
        m_ChunkMeshGenPool.SubmitBatch(std::move(chunksMeshGenInputs));
    }

    void ChunkManager::UnloadOutOfRangeChunks(ChunkCoord playerChunkPos) {
        const int loadDist   = WorldConst::LoadDistance;
        const int renderDist = WorldConst::RenderDistance;

        // Unload Pass
        for (auto it = m_LoadedChunks.begin(); it != m_LoadedChunks.end(); ) {
            ChunkCoord chunkPos = it->first;

            int distX = std::abs(chunkPos.X - playerChunkPos.X);
            int distZ = std::abs(chunkPos.Z - playerChunkPos.Z);

            if (distX > loadDist || distZ > loadDist) {
                it = m_LoadedChunks.erase(it);
                continue;
            }

            ++it;
        }
    }

    std::optional<ChunkMeshInput> ChunkManager::GetChunkMeshInputData(ChunkCoord pos) {
        ChunkMeshInput chunks{
            GetChunk(pos),
            GetChunk({ pos.X, pos.Z - 1 }),
            GetChunk({ pos.X, pos.Z + 1 }),
            GetChunk({ pos.X + 1, pos.Z }),
            GetChunk({ pos.X - 1, pos.Z })
        };

        if (!chunks.IsValid()) {
            return std::nullopt;
        }

        return chunks;
    }

}
