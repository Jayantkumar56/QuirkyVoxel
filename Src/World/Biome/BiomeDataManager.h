

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "World/Block/Block.h"

#include <string>
#include <vector>


namespace Mct {

    using BiomeId = uint8_t;

    struct BiomeTreeEntry {
        uint8_t Type;
        uint8_t Weight; // Higher number = more frequent
    };

    struct BiomeFloraEntry {
        uint8_t Type;
        uint8_t Weight; // Higher number = more frequent
    };

    enum class BiomeLayerBlendMode : uint8_t {
        None,               // Hard edge (Exact numbers)
        Dither_Low,         // +/- 1 block variance (High frequency noise)
        Dither_Medium,      // +/- 2-3 blocks variance
        Dither_High,        // +/- 5 blocks variance (Low frequency noise)
        Wavy_Strata         // Large, sweeping waves (for Badlands/Terracotta)
    };

    struct BiomeLayer {
        BlockId Block;

        // RELATIVE Constraint: Distance from top (HeightMap value)
        // Set to 255 (or WorldHeight) to act as a "Filler" or "Strata"
        int Depth = 1;

        // ABSOLUTE Constraints: World Y coordinates
        int MinAllowedY = -64;
        int MaxAllowedY = 320;

        BiomeLayerBlendMode BlendMode = BiomeLayerBlendMode::None;
    };

    struct BiomeData {
        std::string Name;

        // The Hex Color (0xRRGGBB) used in the GIMP Climate Map.
        uint32_t LutColor = 0xFF00FF; // Default Magenta (Error color)
 
        // The starting Y-level for this biome.
        int BaseHeight = 64;

        // How much the height varies from the BaseHeight.
        float HeightAmplitude = 10.0f;

        // Controls the "smoothness" of the terrain.
        float TerrainRoughness = 0.001f; 

        // The list of Block layers allowed.
        // Priority depends on the order of the blocks in the list.
        std::vector<BiomeLayer> BlockPalette;

        // Probability (0.0 to 1.0) of a tree spawning on a valid block.
        float TreeDensity = 0.0f;

        // The list of trees allowed in this biome and their relative rarity. If empty, no trees spawn.
        std::vector<BiomeTreeEntry> TreePalette;

        // Probability (0.0 to 1.0) of tall grass/flowers spawning.
        float FloraDensity = 0.0f;

        // The list of plants allowed here and their relative frequency.
        std::vector<BiomeFloraEntry> FloraPalette;
    };

    class BiomeDataManager {
        struct BiomeLookupMap {
            std::vector<BiomeId> Ids;
            size_t Width  = 0;
            size_t Height = 0;

            constexpr bool IsValid() const { return !Ids.empty() && Width * Height == Ids.size(); }
        };

    public:
        static bool Init();

        [[nodiscard]] static BiomeId GetLandBiomeId(float temp, float humidity) {
            return SampleMap(s_LandMap, temp, humidity);
        }

        [[nodiscard]] static BiomeId GetOceanBiomeId(float temp, float vegetation) {
            return SampleMap(s_OceanMap, temp, vegetation);
        }

        [[nodiscard]] static const BiomeData& GetBiomeData(uint8_t biomeId) {
            if (biomeId >= s_BiomeData.size())
                return s_BiomeData[0];

            return s_BiomeData[biomeId];
        }

    private:
        [[nodiscard]] static bool LoadAndBakeMap(std::string_view path,
                                                 BiomeLookupMap&  outMap, 
                                                 std::unordered_map<uint32_t, uint8_t>& colorTobiomeId);

        [[nodiscard]] static BiomeId SampleMap(const BiomeLookupMap& map, float xVal, float yVal);

    private:
        inline static std::vector<BiomeData> s_BiomeData;

        // BiomeId is just the index of biome in s_BiomeData
        inline static std::unordered_map<std::string, BiomeId> s_BiomeNameToId;

        // Land BiomeId lookup map from temperature-humidity.
        inline static BiomeLookupMap s_LandMap;

        // Ocean BiomeId lookup map from temperature-vegetation.
        inline static BiomeLookupMap s_OceanMap;
    };

}
