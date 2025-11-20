

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "BlockDataManager.h"
#include "Utils/IdGenerator.h"
#include "Utils/Assert.h"

#include <yaml-cpp/yaml.h>

#include <unordered_map>
#include <iostream>


namespace Mct {

    static bool DecodeBlockData(const YAML::Node&            node,
                                Mct::BlockData&              info, 
                                LinearIdGenerator<uint16_t>& texIdGenerator,
                                std::unordered_map<std::string, uint16_t>& texFileIdMap);

	void BlockDataManager::Init() {
        constexpr std::string_view blockDataFilePath = "Assets/BlockData.yaml";

        YAML::Node root;

        try {
            root = YAML::LoadFile(blockDataFilePath.data());
        }
        catch (const YAML::BadFile& e) {
            std::cerr << "Failed to open YAML file: " << e.what() << "\n";
            return;
        }
        catch (const YAML::ParserException& e) {
            std::cerr << "YAML parse error: " << e.what() << "\n";
            return;
        }
        catch (const YAML::Exception& e) {
            std::cerr << "YAML error: " << e.what() << "\n";
            return;
        }

        LinearIdGenerator<uint16_t> texIdGenerator{0};
        std::unordered_map<std::string, uint16_t> texFileIdMap;

        if (const YAML::Node& blockArr = root["BlocksInfo"]; blockArr && blockArr.IsSequence()) {
            // To generate id for blocks (ids are just index of the block in s_BlockData).
            LinearIdGenerator<BlockId> blockIdGenerator{0};

            s_BlockData.reserve(blockArr.size());

            for (const YAML::Node& item : blockArr) {
                const std::string& blockName = item["BlockType"].Scalar();

                if (s_BlockNameToId.contains(blockName)) {
                    std::cout << "Block: " << blockName << " have duplicate entries in Block DataBase."
                              << " Taking the first entry.";
                    continue;
                }

                s_BlockNameToId[blockName] = blockIdGenerator.GetNext();

                BlockData& currBlockData = s_BlockData.emplace_back();

                if (!DecodeBlockData(item, currBlockData, texIdGenerator, texFileIdMap)) {
                    std::cout << "Unformatted or incomplete data found for Block: " << blockName << " in Block DataBase.";
                }
            }
        }

        // Store unique texture file paths in s_TextureFilePaths.
        {
            s_TextureFilePaths.resize(texFileIdMap.size());

            for (auto& [path, id] : texFileIdMap) {
                s_TextureFilePaths[id] = path;
            }
        }

        // Store ids of core blocks
        {
            CoreBlocks::Storage::Air     = s_BlockNameToId[ "Air"     ];
            CoreBlocks::Storage::Bedrock = s_BlockNameToId[ "Bedrock" ];
            CoreBlocks::Storage::Stone   = s_BlockNameToId[ "Stone"   ];
            CoreBlocks::Storage::Dirt    = s_BlockNameToId[ "Dirt"    ];
            CoreBlocks::Storage::Grass   = s_BlockNameToId[ "Grass"   ];
            CoreBlocks::Storage::Water   = s_BlockNameToId[ "Water"   ];
        }
	}

    static bool DecodeBlockData(const YAML::Node&           node,
                                Mct::BlockData&             info, 
                                LinearIdGenerator<uint16_t>& texIdGenerator,
                                std::unordered_map<std::string, uint16_t>& texFileIdMap) {
        if (!node.IsMap()) return false;

        // IsSolid
        info.IsSolid = node["IsSolid"] ? node["IsSolid"].as<bool>() : false;

        // FaceTextures
        {
            if (!node["FaceTextures"] || !node["FaceTextures"].IsMap()) 
                return false;

            const YAML::Node& faceTextures = node["FaceTextures"];

            const std::string& TopTexture    = faceTextures["Top"   ].Scalar();
            const std::string& BottomTexture = faceTextures["Bottom"].Scalar();
            const std::string& EastTexture   = faceTextures["East"  ].Scalar();
            const std::string& WestTexture   = faceTextures["West"  ].Scalar();
            const std::string& SouthTexture  = faceTextures["South" ].Scalar();
            const std::string& NorthTexture  = faceTextures["North" ].Scalar();

            static auto getOrCreateTextureId = +[](std::unordered_map<std::string, uint16_t>& map,
                                                   const std::string&           textureName,
                                                   LinearIdGenerator<uint16_t>& idGen) {
                auto [it, inserted] = map.try_emplace(textureName, 0);

                if (inserted) {
                    it->second = idGen.GetNext();
                }

                return it->second;
            };

            using enum CubeNormal;
            auto& faceTexIds = info.FaceTextureIds;
            faceTexIds[static_cast<int>(Top   )] = getOrCreateTextureId(texFileIdMap, TopTexture,    texIdGenerator);
            faceTexIds[static_cast<int>(Bottom)] = getOrCreateTextureId(texFileIdMap, BottomTexture, texIdGenerator);
            faceTexIds[static_cast<int>(East  )] = getOrCreateTextureId(texFileIdMap, EastTexture,   texIdGenerator);
            faceTexIds[static_cast<int>(West  )] = getOrCreateTextureId(texFileIdMap, WestTexture,   texIdGenerator);
            faceTexIds[static_cast<int>(South )] = getOrCreateTextureId(texFileIdMap, SouthTexture,  texIdGenerator);
            faceTexIds[static_cast<int>(North )] = getOrCreateTextureId(texFileIdMap, NorthTexture,  texIdGenerator);
        }

        // FaceUV
        {
            if (!node["FaceUV"] || !node["FaceUV"].IsMap()) 
                return false;

            const YAML::Node& faceUVNodes = node["FaceUV"];

            static auto getfaceUV = +[](const YAML::Node& node) -> std::array<glm::vec2, 4> {
                std::array<glm::vec2, 4> faceUV{};

                MCT_ASSERT(node.IsSequence() && "UV node must be a sequence.");
                MCT_ASSERT(node.size() == 4 && "UV sequence must have exactly 4 elements.");

                for (size_t i = 0; i < 4; ++i) {
                    const auto& uv_node = node[i];

                    MCT_ASSERT(uv_node.IsMap() && "UV element must be a map.");
                    MCT_ASSERT(uv_node["u"] && uv_node["v"] && "UV map must have 'u' and 'v' keys.");

                    faceUV[i] = glm::vec2(
                        uv_node["u"].as<float>(),
                        uv_node["v"].as<float>()
                    );
                }

                return faceUV;
            };

            using enum CubeNormal;
            auto& faceUVs = info.FaceUV;
            faceUVs[static_cast<int>(Top   )] = getfaceUV(faceUVNodes["Top"   ]);
            faceUVs[static_cast<int>(Bottom)] = getfaceUV(faceUVNodes["Bottom"]);
            faceUVs[static_cast<int>(East  )] = getfaceUV(faceUVNodes["East"  ]);
            faceUVs[static_cast<int>(West  )] = getfaceUV(faceUVNodes["West"  ]);
            faceUVs[static_cast<int>(South )] = getfaceUV(faceUVNodes["South" ]);
            faceUVs[static_cast<int>(North )] = getfaceUV(faceUVNodes["North" ]);
        }

        return true;
    }

}
