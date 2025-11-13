

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <Utils/Assert.h>

#include <string_view>
#include <unordered_map>


namespace Mct {

	enum class BlockType : uint16_t {
		Air,
		Bedrock,
		Water,
		Dirt,
		Grass,
		Stone,
		COUNT                // Total number of Block types.
	};

	inline const std::unordered_map<std::string_view, BlockType> StringToBlockTypeMap = {
		{   "Air",				             BlockType::Air			},
		{   "Bedrock",			             BlockType::Bedrock		},
		{   "Water",	    	             BlockType::Water		},
		{   "Dirt",				             BlockType::Dirt	    },
		{   "Grass",			             BlockType::Grass		},
		{   "Stone",			             BlockType::Stone		},
	};

	inline const std::unordered_map<BlockType, std::string_view> BlockTypeToStringMap = {
		{   BlockType::Air,                 "Air"            },
		{   BlockType::Bedrock,             "Bedrock"        },
		{   BlockType::Water,               "Water"          },
		{   BlockType::Dirt,                "Dirt"           },
		{   BlockType::Grass,               "Grass"          },
		{   BlockType::Stone,               "Stone"          },
	};

	inline BlockType BlockTypeFromString(std::string_view str) {
		auto it = StringToBlockTypeMap.find(str);
		if (it != StringToBlockTypeMap.end())
			return it->second;
		MCT_ASSERT(false && "Invalid BlockType string provided.");
	}

	inline std::string_view BlockTypeToString(BlockType type) {
		auto it = BlockTypeToStringMap.find(type);
		if (it != BlockTypeToStringMap.end())
			return it->second;
		MCT_ASSERT(false && "Invalid BlockType provided.");
	}

	namespace {
		const bool _ValidateBlockTypeMaps = []() {
			const size_t expected = static_cast<size_t>(BlockType::COUNT);
			MCT_ASSERT(StringToBlockTypeMap.size() == expected && "StringToBlockTypeMap size mismatch");
			MCT_ASSERT(BlockTypeToStringMap.size() == expected && "BlockTypeToStringMap size mismatch");
			return true;
		}();
	}

}
