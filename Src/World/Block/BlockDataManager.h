

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockId.h"
#include "Utils/CubeData.h"

#include <glm/glm.hpp>

#include <vector>
#include <array>
#include <string>
#include <optional>


namespace Mct {

    struct BlockData {
        std::string Name;

        bool IsSolid = false;

        // Store TextureIds for each face in face order Top, Bottom, East, West, South, North.
        std::array<int32_t, static_cast<int>(CubeNormal::COUNT)> FaceTextureIds{};

        // For each face store 4 UVs
        std::array<std::array<glm::vec2, 4>, static_cast<int>(CubeNormal::COUNT)> FaceUV{};
    };

    class BlockDataManager {
    public:
        static void Init();

        static const std::vector<std::string> GetTextureFilePaths() noexcept {
            return s_TextureFilePaths;
        }

        [[nodiscard]] static const BlockData& Get(BlockId type) noexcept {
            return s_BlockData[static_cast<int>(type)];
        }

        [[nodiscard]] static bool BlockHaveMesh(BlockId type) noexcept {
            return type != CoreBlocks::Air;
        }
          
        [[nodiscard]] static bool IsSolid(BlockId type) noexcept {
            return s_BlockData[static_cast<int>(type)].IsSolid;
        }

        [[nodiscard]] static uint32_t GetFaceTexture(BlockId blockType, CubeNormal faceNormal) noexcept {
            const int normalIdx = static_cast<int>(faceNormal);
            const int blockIdx  = static_cast<int>(blockType);
            return s_BlockData[blockIdx].FaceTextureIds[normalIdx];
        }

        [[nodiscard]] static const std::array<glm::vec2, 4>& GetUV(BlockId  blockType,
                                                                   CubeNormal faceNormal) noexcept {
            const int normalIdx = static_cast<int>(faceNormal);
            const int blockIdx  = static_cast<int>(blockType);
            return s_BlockData[blockIdx].FaceUV[normalIdx];
        }

        [[nodiscard]] static std::optional<BlockId> BlockIdFromName(const std::string& blockName) {
            auto it = s_BlockNameToId.find(blockName);

            if (it != s_BlockNameToId.end()) {
                return it->second;
            }

            return std::nullopt;
        }

    private:
        inline static std::vector<std::string> s_TextureFilePaths;
        inline static std::vector<BlockData> s_BlockData;

        // BlockId is just the index of biome in s_BlockData
        inline static std::unordered_map<std::string, BlockId> s_BlockNameToId;
    };

}
