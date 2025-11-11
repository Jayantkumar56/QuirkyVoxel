

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <cassert>


#if defined(MCT_DEBUG)
	#define MCT_ASSERT(_EXPR_) assert(_EXPR_)
#else
	#define MCT_ASSERT(_EXPR_)
#endif
