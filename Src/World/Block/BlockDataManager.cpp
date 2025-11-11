

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "BlockDataManager.h"
#include "Utils/IdGenerator.h"

#include <yaml-cpp/yaml.h>

#include <unordered_map>


namespace Mct {

    static bool DecodeBlockData(const YAML::Node&           node,
                                Mct::BlockData&             info, 
                                LinearIdGenerator<int32_t>& idGenerator,
                                std::unordered_map<std::string, int32_t>& texFileIdMap);

	void BlockDataManager::Init() {
        YAML::Node root = YAML::LoadFile("Assets/BlockInfo.yaml");

        LinearIdGenerator<int> idGenerator{};
        std::unordered_map<std::string, int32_t> texFileIdMap;

        if (root["BlocksInfo"] && root["BlocksInfo"].IsSequence()) {
            const YAML::Node& arr = root["BlocksInfo"];

            for (const auto& item : arr) {
                const auto blockType = BlockTypeFromString(item["BlockType"].Scalar());
                const int  blockIdx  = static_cast<int>(blockType);

                auto& currBlockData = s_BlockData[blockIdx];

                DecodeBlockData(item, currBlockData, idGenerator, texFileIdMap);
            }
        }

        // Store unique texture file paths in s_TextureFilePaths.
        {
            s_TextureFilePaths.resize(texFileIdMap.size());

            for (auto& [path, id] : texFileIdMap) {
                s_TextureFilePaths[id] = std::move(path);
            }
        }
	}

    static bool DecodeBlockData(const YAML::Node&           node,
                                Mct::BlockData&             info, 
                                LinearIdGenerator<int32_t>& idGenerator,
                                std::unordered_map<std::string, int32_t>& texFileIdMap) {
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

            static auto getOrCreateTextureId = +[](std::unordered_map<std::string, int32_t>& map,
                                                   const std::string&      textureName,
                                                   LinearIdGenerator<int32_t>& idGen) -> int32_t {
                auto [it, inserted] = map.try_emplace(textureName, 0);

                if (inserted) {
                    it->second = idGen.GetNext();
                }

                return it->second;
            };

            using enum CubeNormal;
            auto& faceTexIds = info.FaceTextureIds;
            faceTexIds[static_cast<int>(Top   )] = getOrCreateTextureId(texFileIdMap, TopTexture,    idGenerator);
            faceTexIds[static_cast<int>(Bottom)] = getOrCreateTextureId(texFileIdMap, BottomTexture, idGenerator);
            faceTexIds[static_cast<int>(East  )] = getOrCreateTextureId(texFileIdMap, EastTexture,   idGenerator);
            faceTexIds[static_cast<int>(West  )] = getOrCreateTextureId(texFileIdMap, WestTexture,   idGenerator);
            faceTexIds[static_cast<int>(South )] = getOrCreateTextureId(texFileIdMap, SouthTexture,  idGenerator);
            faceTexIds[static_cast<int>(North )] = getOrCreateTextureId(texFileIdMap, NorthTexture,  idGenerator);
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
