

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "SimpleTerrainGen.h"
#include "World/Block/Block.h"
#include "World/Chunk/Chunk.h"

#include <FastNoiseLite/FastNoiseLite.h>


namespace Mct {

	void SimpleTerrainGen::GenerateFor(Chunk& chunk) {
		const HeightMap  heightMap   = CreateHeightMap(chunk);
		ChunkSpan<Block> chunkBlocks = chunk.GetBlocksForWrite();

		chunkBlocks.ForEachXYZ([&heightMap](size_t x, size_t y, size_t z, Block& block) {
			const float terrainHeight = heightMap[x][z];

			if (y > terrainHeight) {
				block = Block(CoreBlocks::Air);
			}
			else if (y == terrainHeight) {
				block = Block(CoreBlocks::Grass);
			}
			else if (y > terrainHeight - 4.0f) {
				block = Block(CoreBlocks::Dirt);
			}
			else {
				block = Block(CoreBlocks::Stone);
			}
		});
	}

	SimpleTerrainGen::HeightMap SimpleTerrainGen::CreateHeightMap(Chunk& chunk) {
		const int seed = 4346;
		const int fractalOctaves = 8;

		FastNoiseLite noise(seed);
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		noise.SetFractalType(FastNoiseLite::FractalType_PingPong);
		noise.SetFractalOctaves(fractalOctaves);

		noise.SetFrequency(0.001f);

		HeightMap heightMap{};

		glm::vec3 chunkPos = chunk.GetCoord().ToBlockCoordFloat();

		for (size_t x = 0; x < WorldConst::ChunkSizeX; ++x) {
			for (size_t z = 0; z < WorldConst::ChunkSizeZ; ++z) {
				const float globalWorldX = (float)(x + chunkPos.x);
				const float globalWorldZ = (float)(z + chunkPos.z);

				const float noiseValue       = noise.GetNoise(globalWorldX, globalWorldZ);
				const float normalizedHeight = (noiseValue + 1.0f) / 2.0f;

				heightMap[x][z] = 64 + (int)(normalizedHeight * 150);
			}
		}
		 
		return heightMap;
	}

}
