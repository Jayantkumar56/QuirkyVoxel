

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <spdlog/spdlog.h>

#include <iostream>
#include <source_location>


namespace Mct {

    struct Log {
        static void Init();
        static void Shutdown();

        template <typename ...Args>
        static void LogMessage(spdlog::level::level_enum   level,
                               const std::source_location& loc,
                               fmt::format_string<Args...> fmt, 
                               Args&&                      ...args) noexcept
        {
            try {
                spdlog::source_loc spdlogLoc{
                    loc.file_name(),
                    static_cast<int>(loc.line()),
                    loc.function_name()
                };

                auto* logger = spdlog::default_logger_raw();
                if (logger) [[likely]] {
                    logger->log(spdlogLoc, level, fmt, std::forward<Args>(args)...);
                }
            }
            catch (const std::exception& e) {
                // EMERGENCY FALLBACK
                std::cerr << "[LOG FAILURE] " << e.what() << "\n";
            }
            catch (...) {
                std::cerr << "[LOG FAILURE] Unknown error occurred.\n";
            }
        }
    };

}


#define MCT_TRACE(...) ::Mct::Log::LogMessage(spdlog::level::trace,    std::source_location::current(), __VA_ARGS__)
#define MCT_INFO(...)  ::Mct::Log::LogMessage(spdlog::level::info,     std::source_location::current(), __VA_ARGS__)
#define MCT_WARN(...)  ::Mct::Log::LogMessage(spdlog::level::warn,     std::source_location::current(), __VA_ARGS__)
#define MCT_ERROR(...) ::Mct::Log::LogMessage(spdlog::level::err,      std::source_location::current(), __VA_ARGS__)
#define MCT_FATAL(...) ::Mct::Log::LogMessage(spdlog::level::critical, std::source_location::current(), __VA_ARGS__)
