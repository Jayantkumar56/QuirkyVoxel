

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace Mct {

    void Log::Init() {
        // Create Sinks
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/MCT_LOG.txt", true);

        // Console Pattern: 
        // Gray Time | Level Color | Level Name | Reset Color | Gray File:Line | Reset | Message
        consoleSink->set_pattern("\033[90m[%H:%M:%S.%e]\033[0m %^[%l]%$ \033[90m[%s:%#]\033[0m %v");

        // File Pattern:
        fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");

        // Create Logger
        std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileSink };
        auto logger = std::make_shared<spdlog::logger>("MCT", sinks.begin(), sinks.end());

        // Register
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::err);        // Auto flush on error
    }

    void Log::Shutdown() {
        spdlog::shutdown();
    }

}
