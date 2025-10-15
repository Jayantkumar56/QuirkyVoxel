

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "TerrainGenerator.h"
#include "SuperFlatTerrainGen.h"


namespace Mct {

    std::unique_ptr<TerrainGenerator> TerrainGenerator::Create(const TerrainType type) {
        switch (type) {
            case TerrainType::SuperFlat: return std::make_unique<SuperFlatTerrainGen>();

            default: return nullptr;
        }

        return nullptr;
    }

}
