

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Utils/Assert.h"
#include "Image.h"

#include <stb/stb_image.h>


namespace Mct {

    Image::Image(const std::string& filepath, bool flipVertically, int forceChannels) {
        stbi_set_flip_vertically_on_load(flipVertically);

        m_Data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, forceChannels);

        MCT_ASSERT(m_Data != nullptr && "Image Failed to load");
    }

    Image::~Image() {
        if (m_Data) {
            stbi_image_free(m_Data);
        }
    }

    Image::Image(Image&& other) noexcept : 
            m_Data    ( other.m_Data     ),
            m_Width   ( other.m_Width    ),
            m_Height  ( other.m_Height   ),
            m_Channels( other.m_Channels ) {
        other.m_Data = nullptr;
    }

    Image& Image::operator=(Image&& other) noexcept {
        if (this != &other) {
            if (m_Data) {
                stbi_image_free(m_Data);
            }

            m_Data     = other.m_Data;
            m_Width    = other.m_Width;
            m_Height   = other.m_Height;
            m_Channels = other.m_Channels;

            other.m_Data = nullptr;
        }

        return *this;
    }

}
