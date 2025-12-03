

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <string>
#include <optional>
#include <fstream>
#include <filesystem>


namespace Mct::Utils {

    inline std::optional<std::string> ReadFileToString(const std::filesystem::path& filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            return std::nullopt;
        }

        const size_t fileSize = static_cast<size_t>(file.tellg());

        std::string buffer;
        buffer.resize(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        return buffer;
    }

}
