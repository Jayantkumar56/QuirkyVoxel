

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <cstdint>


namespace Mct {

	using BlockId = uint16_t;

	class CoreBlocks {
		// Only BlockDataManager is allowed to touch the private Storage
		friend class BlockDataManager;

	private:
		// The Actual Storage (Mutable)
		// Only accessible by friends
		struct Storage {
			inline static BlockId Air     = 0;
			inline static BlockId Bedrock = 0;
			inline static BlockId Stone   = 0;
			inline static BlockId Dirt    = 0;
			inline static BlockId Grass   = 0;
			inline static BlockId Water   = 0;
		};

	public:
		// The Public Read-Only Interface
		// Everyone uses these. They cannot be assigned to.
		inline static const BlockId& Air     = Storage::Air;
		inline static const BlockId& Bedrock = Storage::Bedrock;
		inline static const BlockId& Stone   = Storage::Stone;
		inline static const BlockId& Dirt    = Storage::Dirt;
		inline static const BlockId& Grass   = Storage::Grass;
		inline static const BlockId& Water   = Storage::Water;

		CoreBlocks() = delete;
	};

}
