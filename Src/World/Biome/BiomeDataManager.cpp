

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Utils/Assert.h"
#include "BiomeDataManager.h"
#include "Utils/IdGenerator.h"
#include "Utils/Image.h"
#include "World/Block/BlockDataManager.h"

#include <yaml-cpp/yaml.h>

#include <iostream>


namespace Mct {

    static BiomeLayerBlendMode BiomeLayerBlendModeFromString(std::string_view blendMode);

    static bool DecodeBiomeData(const YAML::Node& biomeNode, BiomeData& biomeData);

	bool BiomeDataManager::Init() {
        constexpr std::string_view biomeDataFilePath     = "Assets/BiomeData.yaml";
        constexpr std::string_view biomeMapLandFilePath  = "Assets/Textures/BiomeMapLand.png";
        constexpr std::string_view biomeMapOceanFilePath = "Assets/Textures/BiomeMapOcean.png";

        YAML::Node root;

        try {
            root = YAML::LoadFile(biomeDataFilePath.data());
        }
        catch (const YAML::BadFile& e) {
            std::cerr << "Failed to open YAML file: " << e.what() << "\n";
            return false;
        }
        catch (const YAML::ParserException& e) {
            std::cerr << "YAML parse error: " << e.what() << "\n";
            return false;
        }
        catch (const YAML::Exception& e) {
            std::cerr << "YAML error: " << e.what() << "\n";
            return false;
        }

        if (!root || !root.IsMap()) {
            std::cerr << "Invalid biome YAML format.\n";
            return false;
        }

        if (const YAML::Node& biomes = root["BiomesInfo"]; biomes && biomes.IsSequence()) {
            // To generate id for biomes (ids are just index of the biome in s_BiomeData).
            LinearIdGenerator<BiomeId> idGenerator{0};

            s_BiomeData.reserve(biomes.size());

            for (const YAML::Node& biomeNode : biomes) {
                const std::string& biomeName = biomeNode["Biome"].Scalar();

                if (s_BiomeNameToId.contains(biomeName)) {
                    std::cerr << "Biome: " << biomeName << " have duplicate entries in Biome DataBase."
                              << " Taking the first entry.\n";
                    continue;
                }

                s_BiomeNameToId[biomeName] = idGenerator.GetNext();

                BiomeData& currBiomeData = s_BiomeData.emplace_back();
                currBiomeData.Name = biomeName;

                if (!DecodeBiomeData(biomeNode, currBiomeData)) {
                    std::cerr << "Unformatted or incomplete data found for Biome: " << biomeName << " in Biome DataBase.\n";
                }
            }
        }

        // Load biome lookup maps
        {
            std::unordered_map<uint32_t, BiomeId> colorToBiomeId;
            colorToBiomeId.reserve(s_BiomeData.size());

            // To get all the biome ids.
            LinearIdGenerator<BiomeId> biomeId(0);

            for (const auto& biome : s_BiomeData) {
                // BiomeId are index of biomes in s_BiomeData.
                colorToBiomeId.emplace(biome.LutColor, biomeId.GetNext());
            }

            if (!LoadAndBakeMap(biomeMapLandFilePath, s_LandMap, colorToBiomeId)) {
                return false;
            }

            if (!LoadAndBakeMap(biomeMapOceanFilePath, s_OceanMap, colorToBiomeId)) {
                return false;
            }
        }

        return true;
    }

    bool BiomeDataManager::LoadAndBakeMap(std::string_view path, 
                                          BiomeLookupMap&  outMap, 
                                          std::unordered_map<uint32_t, BiomeId>& colorTobiomeId) {
        std::optional<Image> optImage = Image::Create(path.data(), true, 4);
        MCT_ASSERT(optImage.has_value(), "First image is not valid.");

        const Image& lut = *optImage;

        outMap.Width  = lut.GetWidth();
        outMap.Height = lut.GetHeight();

        const size_t size = outMap.Width * outMap.Height;
        outMap.Ids.resize(size);

        const uint8_t* pixels = static_cast<const uint8_t*>(lut.GetData());

        for (size_t i = 0; i < size; ++i) {
            const uint8_t r = pixels[i * 3 + 0];
            const uint8_t g = pixels[i * 3 + 1];
            const uint8_t b = pixels[i * 3 + 2];

            const uint32_t hexColor = (r << 16) | (g << 8) | b;

            const auto it = colorTobiomeId.find(hexColor);

            outMap.Ids[i] = it != colorTobiomeId.end() ? it->second : 0;
        }

        return true;
    }

    BiomeId BiomeDataManager::SampleMap(const BiomeLookupMap& map, float xVal, float yVal) {
        MCT_ASSERT(map.IsValid());
        MCT_ASSERT(0.0f < xVal && xVal < 1.0f);
        MCT_ASSERT(0.0f < yVal && yVal < 1.0f);

        // Convert to Pixel Coords
        const size_t x = static_cast<size_t>(xVal * (map.Width  - 1));
        const size_t y = static_cast<size_t>(yVal * (map.Height - 1));

        const size_t index = y * map.Width + x;
        return map.Ids[index];
    }

	static bool DecodeBiomeData(const YAML::Node& biomeNode, BiomeData& biomeData) {
        if (!biomeNode.IsMap())
            return false;

        if (const auto lutColorNode = biomeNode["LutColor"]; lutColorNode) {
            const std::string& hexStr = lutColorNode.Scalar();
            biomeData.LutColor = std::stoul(hexStr, nullptr, 16);
        }

        if (const auto n = biomeNode[ "BaseHeight"       ]; n)  biomeData.BaseHeight       = n.as<int>();
        if (const auto n = biomeNode[ "HeightAmplitude"  ]; n)  biomeData.HeightAmplitude  = n.as<float>();
        if (const auto n = biomeNode[ "TerrainRoughness" ]; n)  biomeData.TerrainRoughness = n.as<float>();

        if (const auto blockLayers = biomeNode["BlockPalette"]; blockLayers && blockLayers.IsSequence()) {
            for (const YAML::Node& layerNode : blockLayers) {
                BiomeLayer& layer = biomeData.BlockPalette.emplace_back();
            
                const std::string&           blockName = layerNode["Block"].Scalar();
                const std::optional<BlockId> blockId   = BlockDataManager::BlockIdFromName(blockName);

                if (!blockId) {
                    std::cerr << "Invalid Block Name " << blockName << "\n";
                    return false;
                }

                layer.Block = *blockId;

                layer.Depth       = layerNode[ "Depth"       ].as<int>();
                layer.MinAllowedY = layerNode[ "MinAllowedY" ].as<int>();
                layer.MaxAllowedY = layerNode[ "MaxAllowedY" ].as<int>();
            
                const std::string_view blendName = layerNode["BlendMode"].Scalar();
                layer.BlendMode = BiomeLayerBlendModeFromString(blendName);
            }
        }

        if (const auto n = biomeNode["TreeDensity"]; n) biomeData.TreeDensity = n.as<float>();
    
        if (const auto treeNodes = biomeNode["TreePalette"]; treeNodes && treeNodes.IsSequence()) {
            for (const auto& treeNode : treeNodes) {
                BiomeTreeEntry& entry = biomeData.TreePalette.emplace_back();

                std::string_view typeName = treeNode["TreeType"].Scalar();
                //entry.Type = TreeTypeFromString(typeName);

                entry.Weight = treeNode["Weight"].as<uint8_t>();
            }
        }

        if (const auto n = biomeNode["FloraDensity"]; n) biomeData.FloraDensity = n.as<float>();

        if (const auto floraNodes = biomeNode["FloraPalette"]; floraNodes && floraNodes.IsSequence()) {
            for (const auto& floraNode : floraNodes) {
                BiomeFloraEntry& entry = biomeData.FloraPalette.emplace_back();

                std::string_view typeName = floraNode["FloraType"].Scalar();
                //entry.Type = FloraTypeFromString(typeName);

                entry.Weight = floraNode["Weight"].as<uint8_t>();
            }
        }

        return true;
    }

    static BiomeLayerBlendMode BiomeLayerBlendModeFromString(const std::string_view blendMode) {
        using enum BiomeLayerBlendMode;

        if (blendMode == "None"          ) return None;
        if (blendMode == "Dither_Low"    ) return Dither_Low;
        if (blendMode == "Dither_Medium" ) return Dither_Medium;
        if (blendMode == "Dither_High"   ) return Dither_High;
        if (blendMode == "Wavy_Strata"   ) return Wavy_Strata;

        std::cerr << "Invalid BiomeLayerBlendMode: \"" << blendMode << "\" provided.";
        return None;
    }

}
