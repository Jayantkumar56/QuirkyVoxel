

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Texture2D.h"

#include <span>
#include <string>


namespace Mct {

    class TextureArray : public NonCopyable {
    public:
        TextureArray(std::span<const std::string> filepaths, TextureProperties properties);
        ~TextureArray();

        TextureArray(TextureArray&& other)            noexcept;
        TextureArray& operator=(TextureArray&& other) noexcept;

        void Bind(uint32_t textureUnit = 0) const noexcept;

        [[nodiscard]] uint32_t          GetRendererID() const noexcept { return m_RendererID; }
        [[nodiscard]] uint32_t          GetWidth()      const noexcept { return m_Width;      }
        [[nodiscard]] uint32_t          GetHeight()     const noexcept { return m_Height;     }
        [[nodiscard]] uint32_t          GetLayerCount() const noexcept { return m_Layers;     }
        [[nodiscard]] TextureProperties GetProperties() const noexcept { return m_Properties; }

    private:
        void Recreate(const void*       data,
                      ImgDataType       dataType,
                      uint32_t          width,
                      uint32_t          height,
                      uint32_t          channels,
                      uint32_t          layers,
                      TextureProperties properties) noexcept;

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_Width      = 0;
        uint32_t m_Height     = 0;
        uint32_t m_Layers     = 0;

        TextureProperties m_Properties;
    };

}
