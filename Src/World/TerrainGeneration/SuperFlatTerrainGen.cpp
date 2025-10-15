

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "SuperFlatTerrainGen.h"
#include "World/Chunk/Chunk.h"

#include <mdspan>


namespace Mct {

	void SuperFlatTerrainGen::GenerateFor(Chunk& chunk) noexcept {
        ChunkSpan<Block> blocks = chunk.GetBlocksForWrite();

        constexpr size_t BEDROCK_Y = 0;
        constexpr size_t GRASS_Y   = 4;     // 1..3 dirt, 4 grass

        for (size_t x = 0; x < blocks.SizeX(); ++x) {
            for (size_t z = 0; z < blocks.SizeZ(); ++z) {
                // Bedrock
                blocks(x, BEDROCK_Y, z) = Block{ BlockType::Bedrock };

                // Dirt layers (protect against small ChunkSizeY just in case)
                for (size_t y = 1; y < 4 && y < blocks.SizeY(); ++y)
                    blocks(x, y, z) = Block{ BlockType::Dirt };

                // Grass on top if available
                if (blocks.SizeY() > GRASS_Y)
                    blocks(x, GRASS_Y, z) = Block{ BlockType::Grass };

                // Air above
                for (size_t y = GRASS_Y + 1; y < blocks.SizeY(); ++y)
                    blocks(x, y, z) = Block{ BlockType::Air };
            }
        }
	}

}
