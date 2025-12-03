

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"
#include "Utils/Image.h"

#include <string_view>


namespace Mct {

    enum class ImgDataFmt : uint8_t {          // Data upload formats
        None = 0,

        Red,
        RG,
        RGB,
        RGBA,

        // Depth and Stencil formats
        Depth,
        Stencil,
    };

    enum class ImgInternalFmt : uint8_t {      // GPU storage format
        None = 0,

        R8,
        RG8,
        RGB8,
        RGBA8,

        R16F,
        RG16F,
        RGB16F,
        RGBA16F,

        R32F,
        RG32F,
        RGB32F,
        RGBA32F,

        SRGB8,
        SRGB8_Alpha8,

        Depth24Stencil8,
        Depth32F
    };

    enum class Swizzle : uint8_t {
        None,
        Red,
        Green,
        Blue,
        Alpha,
        Zero,
        One
    };

    struct TextureSwizzle {
        Swizzle R{ Swizzle::None };
        Swizzle G{ Swizzle::None };
        Swizzle B{ Swizzle::None };
        Swizzle A{ Swizzle::None };

        [[nodiscard]] constexpr bool IsCustom() const noexcept {
            return R != Swizzle::None || G != Swizzle::None ||
                   B != Swizzle::None || A != Swizzle::None;
        }
    };

    enum class TextureFilter : uint8_t {
        Linear,
        Nearest
    };

    enum class TextureWrap : uint8_t {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    struct TextureProperties {
        ImgInternalFmt InternalFormat = ImgInternalFmt::RGBA8;

        // How many levels to generate?
        // 0 -> Auto-calculate maximum possible levels (Full chain)
        // 1 -> Base level only (No mipmaps)
        // greater than 1 -> Specific number of levels
        uint32_t MipLevels = 1;

        TextureFilter MinFilter{ TextureFilter::Linear };
        TextureFilter MagFilter{ TextureFilter::Linear };

        TextureWrap WrapS{ TextureWrap::Repeat };
        TextureWrap WrapT{ TextureWrap::Repeat };

        TextureSwizzle SwizzleMask{ };
    };

    [[nodiscard]] ImgDataFmt     ImgDataFmtFromChannels     (uint32_t channels) noexcept;
    [[nodiscard]] ImgInternalFmt ImgInternalFmtFromChannels (uint32_t channels) noexcept;

    [[nodiscard]] int32_t  TextureWrapToGL    (TextureWrap    wrap  ) noexcept;
    [[nodiscard]] int32_t  TexutureFilterToGL (TextureFilter  filter) noexcept;
    [[nodiscard]] uint32_t ImgDataFmtToGL     (ImgDataFmt     format) noexcept;
    [[nodiscard]] uint32_t ImgInternalFmtToGL (ImgInternalFmt format) noexcept;
    [[nodiscard]] uint32_t ImgDataTypeToGL    (ImgDataType    type  ) noexcept;

    [[nodiscard]] int32_t SwizzleToGLOrDefault(Swizzle swizzle, int32_t defaultSwizzle) noexcept;

    class Texture2D : NonCopyable {
    public:
        Texture2D(std::string_view filePath, TextureProperties properties);
        ~Texture2D() noexcept;

        void Recreate(const Image& image, TextureProperties properties) noexcept;

        Texture2D(Texture2D&& other)            noexcept;
        Texture2D& operator=(Texture2D&& other) noexcept;

        void Bind(uint32_t slot = 0) const noexcept;

        [[nodiscard]] uint32_t          GetWidth()      const noexcept { return m_Width;      }
        [[nodiscard]] uint32_t          GetHeight()     const noexcept { return m_Height;     }
        [[nodiscard]] uint32_t          GetRendererId() const noexcept { return m_RendererID; }
        [[nodiscard]] TextureProperties GetProperties() const noexcept { return m_Properties; }

    private:
        uint32_t m_Width      = 0;
        uint32_t m_Height     = 0;
        uint32_t m_RendererID = 0;

        TextureProperties m_Properties;
    };

}
