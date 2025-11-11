

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "BlockType.h"
#include "BlockDataManager.h"
#include "Utils/CubeData.h"


namespace Mct {

	class Block {
	public:
		constexpr Block(BlockType type = BlockType::Air) noexcept : m_Type(type) {}

		[[nodiscard]] constexpr BlockType GetType() const noexcept { return m_Type; }

		[[nodiscard]] bool IsSolid()  const noexcept { return BlockDataManager::IsSolid(m_Type);       }
		[[nodiscard]] bool IsWater()  const noexcept { return m_Type == BlockType::Water;              }
		[[nodiscard]] bool HaveMesh() const noexcept { return BlockDataManager::BlockHaveMesh(m_Type); }

		[[nodiscard]] uint32_t GetFaceTexture(CubeNormal faceNormal) const noexcept {
			return BlockDataManager::GetFaceTexture(m_Type, faceNormal);
		}

		[[nodiscard]] const std::array<glm::vec2, 4>& GetUV(CubeNormal faceNormal) const noexcept {
			return BlockDataManager::GetUV(m_Type, faceNormal);
		}

	private:
		BlockType m_Type;
	};

}
