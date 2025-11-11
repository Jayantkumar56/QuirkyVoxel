

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockType.h"
#include "Utils/CubeData.h"

#include <glm/glm.hpp>

#include <vector>
#include <array>
#include <string>


namespace Mct {

    struct BlockData {
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

        [[nodiscard]] static const BlockData& Get(BlockType type) noexcept {
            return s_BlockData[static_cast<int>(type)];
        }

        [[nodiscard]] static bool BlockHaveMesh(BlockType type) noexcept {
            return type != BlockType::Air;
        }
          
        [[nodiscard]] static bool IsSolid(BlockType type) noexcept {
            return s_BlockData[static_cast<int>(type)].IsSolid;
        }

        [[nodiscard]] static uint32_t GetFaceTexture(BlockType blockType, CubeNormal faceNormal) noexcept {
            const int normalIdx = static_cast<int>(faceNormal);
            const int blockIdx  = static_cast<int>(blockType);
            return s_BlockData[blockIdx].FaceTextureIds[normalIdx];
        }

        [[nodiscard]] static const std::array<glm::vec2, 4>& GetUV(BlockType  blockType, 
                                                                   CubeNormal faceNormal) noexcept {
            const int normalIdx = static_cast<int>(faceNormal);
            const int blockIdx  = static_cast<int>(blockType);
            return s_BlockData[blockIdx].FaceUV[normalIdx];
        }

    private:
        inline static std::vector<std::string> s_TextureFilePaths;
        inline static std::array<BlockData, static_cast<int>(BlockType::COUNT)> s_BlockData;
    };

}
