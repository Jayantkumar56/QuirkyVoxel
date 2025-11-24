

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Logger.h"


#if defined(_MSC_VER)
#define MCT_DEBUGBREAK() __debugbreak() // MSVC
#elif defined(__GNUC__) || defined(__clang__)
#define MCT_DEBUGBREAK() __builtin_trap() // GCC/Clang
#else
#include <cassert>
#define MCT_DEBUGBREAK() assert(false) // Fallback
#endif

#if defined(MCT_DEBUG)
#define MCT_ASSERT(Expr, ...)                                                                        \
        [&](const char* msg = nullptr,                                                               \
            const std::source_location& loc = std::source_location::current()) noexcept              \
        {                                                                                            \
            if ( !static_cast<bool>(Expr) ) [[unlikely]] {                                           \
                if (std::is_constant_evaluated()) {                                                  \
                    MCT_DEBUGBREAK(); /* Causes: call to non-constexpr function error */             \
                } else {                                                                             \
                    if (msg) {                                                                       \
                        ::Mct::Log::LogMessage(spdlog::level::critical, loc,                         \
                            "ASSERTION FAILED!\n\tExpr: {}\n\tMsg:  {}", #Expr, msg);                \
                    } else {                                                                         \
                        ::Mct::Log::LogMessage(spdlog::level::critical, loc,                         \
                            "ASSERTION FAILED!\n\tExpr: {}", #Expr);                                 \
                    }                                                                                \
                    MCT_DEBUGBREAK();                                                                \
                }                                                                                    \
            }                                                                                        \
        }(__VA_ARGS__);
#else
#define MCT_ASSERT(_EXPR_) ((void)0)
#endif
