

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/Assert.h"

#include <string>
#include <fstream>
#include <sstream>


namespace Mct::Utils {

    inline std::string ReadFileToString(const std::string& filepath) {
        std::ifstream file(filepath);

        MCT_ASSERT(file.is_open() && "Cannot open file.");

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

}
