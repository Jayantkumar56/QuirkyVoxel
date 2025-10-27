

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkManager.h"
#include "TerrainGeneration/TerrainGenerator.h"


namespace Mct {

    std::shared_ptr<Chunk> ChunkManager::GetChunk(ChunkCoord pos) {
        if (auto it = m_LoadedChunks.find(pos); it != m_LoadedChunks.end())
            return it->second;

        return nullptr;
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

    void ChunkManager::Update(glm::vec3 playerPos) {
        UpdateVisibleChunks(playerPos);
    }

    void ChunkManager::UpdateVisibleChunks(glm::vec3 playerPos) {
        ChunkCoord playerChunkPos = ChunkCoord::FromWorldXZ(
            static_cast<int>(std::floor(playerPos.x)),
            static_cast<int>(std::floor(playerPos.z))
        );

        const int renderDist = WorldConst::RenderDistance;

        // --- Load Pass ---
        for (int x = playerChunkPos.X - renderDist; x <= playerChunkPos.X + renderDist; ++x) {
            for (int z = playerChunkPos.Z - renderDist; z <= playerChunkPos.Z + renderDist; ++z) {
                ChunkCoord pos = { x, z };

                // If the chunk doesn't exist, add it
                if (!m_LoadedChunks.contains(pos)) {
                    auto newChunk = std::make_unique<Chunk>(pos);

                    m_TerrainGenerator.GenerateFor(*newChunk);

                    m_LoadedChunks[pos] = std::move(newChunk);
                }
            }
        }

        // --- Unload Pass ---
        for (auto it = m_LoadedChunks.begin(); it != m_LoadedChunks.end(); ) {
            ChunkCoord chunkPos = it->first;

            int distX = std::abs(chunkPos.X - playerChunkPos.X);
            int distZ = std::abs(chunkPos.Z - playerChunkPos.Z);

            if (distX > renderDist || distZ > renderDist) {
                it = m_LoadedChunks.erase(it);
            }
            else {
                ++it;
            }
        }
	}

}
