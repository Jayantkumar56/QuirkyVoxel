

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockType.h"
#include "Renderer/Primitives/CubeData.h"

#include <glm/glm.hpp>

#include <array>


namespace Mct {

    struct BlockData {
        bool IsSolid = false;
    };

    class BlockDataManager {
    public:
        static void Init() {
            s_BlockData[static_cast<int>(BlockType::Air)] = BlockData{
                .IsSolid = false
            };

            s_BlockData[static_cast<int>(BlockType::Bedrock)] = BlockData{
                .IsSolid = true
            };

            s_BlockData[static_cast<int>(BlockType::Water)] = BlockData{
                .IsSolid = false
            };

            s_BlockData[static_cast<int>(BlockType::Dirt)] = BlockData{
                .IsSolid = true
            };

            s_BlockData[static_cast<int>(BlockType::Grass)] = BlockData{
                .IsSolid = true
            };
        }

        static const BlockData& Get(BlockType type) noexcept {
            return s_BlockData[static_cast<int>(type)];
        }

        static bool BlockHaveMesh(BlockType type) noexcept {
            return type != BlockType::Air;
        }

        static bool IsSolid(BlockType type) noexcept {
            return s_BlockData[static_cast<int>(type)].IsSolid;
        }

        [[nodiscard]] static uint32_t GetFaceTexture(BlockType blockType, CubeNormal faceNormal) noexcept {
            return 0;       // TODO: Return proper texture id.
        }

        [[nodiscard]] static const std::array<glm::uvec2, 4>& GetUV(BlockType blockType, CubeNormal faceNormal) noexcept {
            // TODO: Remove this temp and Return proper texture uv.
            static std::array<glm::uvec2, 4> temp{};
            return temp;
        }

    private:
        inline static BlockData s_BlockData[static_cast<int>(BlockType::COUNT)];
    };

}
