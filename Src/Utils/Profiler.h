

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#if defined(TRACY_ENABLE)

#include <tracy/Tracy.hpp>

// Mark the start of a frame (Call this ONCE per frame loop).
#define MCT_PROFILE_FRAME(Name)    FrameMarkNamed(Name)

// Profile a specific scope with a custom name (e.g. "Physics Step").
#define MCT_PROFILE_SCOPE(Name)    ZoneScopedN(Name)

// Profile a function automatically using its real name.
#define MCT_PROFILE_FUNCTION()     ZoneScoped

// Plot a value over time (e.g. "Entities", "Memory Usage").
#define MCT_PROFILE_VALUE(Name, Val) TracyPlot(Name, Val)

// Profile a lock/mutex (If debugging thread contention).
#define MCT_PROFILE_LOCK(Type, Var, Name) Type Var; TracyLockableN(Type, Var, Name)

#else

#define MCT_PROFILE_FRAME(Name)
#define MCT_PROFILE_SCOPE(Name)
#define MCT_PROFILE_FUNCTION()
#define MCT_PROFILE_VALUE(Name, Val)
#define MCT_PROFILE_LOCK(Type, Var, Name) Type Var

#endif
