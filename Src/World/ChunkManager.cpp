

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "ChunkManager.h"


namespace Mct {

	// Initalise static Members

	std::array<Block, WorldConst::SubchunkBlockCount> ChunkManager::s_EmptySubchunkData;
	Subchunk ChunkManager::s_EmptySubchunk{ ChunkManager::s_EmptySubchunkData.data() };


	// ChunkManager Implementation

}
