

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/NonCopyable.h"

#include <span>
#include <string>


namespace Mct {

    class TextureArray : public NonCopyable {
    public:
        TextureArray(std::span<const std::string> filepaths);
        ~TextureArray();

        TextureArray(TextureArray&& other)            noexcept;
        TextureArray& operator=(TextureArray&& other) noexcept;

        // Generates mipmaps for the entire texture array.
        // Call this after all slices have been loaded.
        void GenerateMipmaps() const noexcept;

        void Bind(uint32_t textureUnit = 0)   const noexcept;
        void Unbind(uint32_t textureUnit = 0) const noexcept;

        [[nodiscard]] uint32_t GetRendererID() const noexcept { return m_RendererID; }
        [[nodiscard]] uint32_t GetWidth()      const noexcept { return m_Width;      }
        [[nodiscard]] uint32_t GetHeight()     const noexcept { return m_Height;     }
        [[nodiscard]] uint32_t GetLayerCount() const noexcept { return m_Layers;     }

    private:
        uint32_t m_RendererID = 0;
        uint32_t m_Width      = 0;
        uint32_t m_Height     = 0;
        uint32_t m_Layers     = 0;
    };

}
