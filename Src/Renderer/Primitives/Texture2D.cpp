

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Texture2D.h"
#include "Utils/Assert.h"
#include "Utils/TextureUtils.h"

#include <glad/glad.h>

#include <utility>


namespace Mct {

    Texture2D::Texture2D(std::string_view filePath, TextureProperties properties) {
        std::optional<Image> optImage = Image::Create(filePath.data());

        if (!optImage) {
            MCT_ERROR("Failed to load: {}", filePath);

            properties.MipLevels = 1;
            properties.MinFilter = TextureFilter::Nearest;
            properties.MagFilter = TextureFilter::Nearest;
            properties.WrapS     = TextureWrap::Repeat;
            properties.WrapT     = TextureWrap::Repeat;
        }

        const Image& image = optImage ? *optImage : TextureUtils::GetMissingTexture();

        Recreate(image, properties);
    }

    Texture2D::~Texture2D() noexcept {
        if (m_RendererID) {
            glDeleteTextures(1, &m_RendererID);
        }
    }

    void Texture2D::Recreate(const Image& image, TextureProperties properties) noexcept {
        if (m_RendererID) {
            glDeleteTextures(1, &m_RendererID);
        }

        m_Width      = image.GetWidth();
        m_Height     = image.GetHeight();
        m_Properties = properties;

        const ImgDataFmt     dataFormat     = ImgDataFmtFromChannels(image.GetChannels());
        const ImgInternalFmt internalFormat = properties.InternalFormat != ImgInternalFmt::None ?
                                              properties.InternalFormat                         :
                                              ImgInternalFmtFromChannels(image.GetChannels());

        const GLenum glDataFormat     = ImgDataFmtToGL(dataFormat);
        const GLenum glInternalFormat = ImgInternalFmtToGL(internalFormat);

        const GLsizei maxPossibleLevels = static_cast<GLsizei>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;
        const GLsizei requestedLevels   = (properties.MipLevels == 0) ?
                                          maxPossibleLevels :
                                          static_cast<GLsizei>(properties.MipLevels);

        // If MipLevels is 0, use max. Otherwise, clamp user input to max.
        const GLsizei mipLevels = std::min(requestedLevels, maxPossibleLevels);

        const GLenum glUploadType = ImgDataTypeToGL(image.GetDataType());
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, mipLevels, glInternalFormat, m_Width, m_Height);

        if (dataFormat != ImgDataFmt::RGBA) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        glTextureSubImage2D(
            m_RendererID,
            0, 0, 0,
            m_Width,
            m_Height,
            glDataFormat,
            glUploadType,
            image.GetData()
        );

        if (dataFormat != ImgDataFmt::RGBA) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }

        if (mipLevels > 1) {
            glGenerateTextureMipmap(m_RendererID);
        }
        
        const GLint glMagFilter = TexutureFilterToGL(properties.MagFilter);
        GLint       glMinFilter = TexutureFilterToGL(properties.MinFilter);

        if (mipLevels > 1) {
            glMinFilter = properties.MinFilter == TextureFilter::Nearest ?
                                                  GL_NEAREST_MIPMAP_LINEAR :
                                                  GL_LINEAR_MIPMAP_LINEAR;
        }
        
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, glMinFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, glMagFilter);
        
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapToGL(properties.WrapS));
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapToGL(properties.WrapT));

        if (properties.SwizzleMask.IsCustom()) {
            const GLint swizzleMask[] = {
                SwizzleToGLOrDefault(properties.SwizzleMask.R, GL_RED  ),   // Default R -> Red
                SwizzleToGLOrDefault(properties.SwizzleMask.G, GL_GREEN),   // Default G -> Green
                SwizzleToGLOrDefault(properties.SwizzleMask.B, GL_BLUE ),   // Default B -> Blue
                SwizzleToGLOrDefault(properties.SwizzleMask.A, GL_ALPHA)    // Default A -> Alpha
            };

            glTextureParameteriv(m_RendererID, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
        }
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept :
            m_RendererID ( std::exchange(other.m_RendererID, 0) ),
            m_Width      ( std::exchange(other.m_Width,      0) ),
            m_Height     ( std::exchange(other.m_Height,     0) ),
            m_Properties ( other.m_Properties                   )
    {}

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
        if (this != &other) {
            if (m_RendererID) {
                glDeleteTextures(1, &m_RendererID);
            }

            m_RendererID = std::exchange(other.m_RendererID, 0);
            m_Width      = std::exchange(other.m_Width,      0);
            m_Height     = std::exchange(other.m_Height,     0);
            m_Properties = other.m_Properties;
        }

        return *this;
    }

    void Texture2D::Bind(uint32_t slot) const noexcept {
        glBindTextureUnit(slot, m_RendererID);
    }

    //=============================================================================================================================
    //---------------- Texture2D enum to OpenGL enum conversions ------------------------------------------------------------------

    ImgDataFmt ImgDataFmtFromChannels(uint32_t channels) noexcept {
        switch (channels) {
            case 1 :  return ImgDataFmt::Red;
            case 2 :  return ImgDataFmt::RG;
            case 3 :  return ImgDataFmt::RGB;
            case 4 :  return ImgDataFmt::RGBA;
        }

        MCT_ASSERT(false, "Invalid no of channels for ImgDataFmt.");
        return ImgDataFmt::None;
    }

    ImgInternalFmt ImgInternalFmtFromChannels(uint32_t channels) noexcept {
        switch (channels) {
            case 1 :  return ImgInternalFmt::R8;
            case 2 :  return ImgInternalFmt::RG8;
            case 3 :  return ImgInternalFmt::RGB8;
            case 4 :  return ImgInternalFmt::RGBA8;
        }

        MCT_ASSERT(false, "Invalid no of channels for ImgInternalFmt.");
        return ImgInternalFmt::None;
    }

    uint32_t ImgDataFmtToGL(ImgDataFmt format) noexcept {
		switch (format) {
            case ImgDataFmt::RGB:  return GL_RGB;
			case ImgDataFmt::RGBA: return GL_RGBA;
		}

		MCT_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

	uint32_t ImgInternalFmtToGL(ImgInternalFmt format) noexcept {
		switch (format) {
			case ImgInternalFmt::RGB8:  return GL_RGB8;
			case ImgInternalFmt::RGBA8: return GL_RGBA8;
		}

        MCT_ASSERT(false, "Incorrect Image Format!");
		return 0;
	}

    uint32_t ImgDataTypeToGL(ImgDataType type) noexcept{
        switch (type) {
            case ImgDataType::U8:    return GL_UNSIGNED_BYTE;
            case ImgDataType::U16:   return GL_UNSIGNED_SHORT;
            case ImgDataType::F32:   return GL_FLOAT;
        }

        MCT_ASSERT(false, "Invalid TextureWrap type choosen!");
        return 0;
    }

    int32_t TextureWrapToGL(TextureWrap wrap) noexcept {
        switch (wrap) {
            case TextureWrap::Repeat:         return GL_REPEAT;
            case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
            case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        }

        MCT_ASSERT(false, "Invalid TextureWrap type choosen!");
        return -1;
    }

    int32_t TexutureFilterToGL(TextureFilter filter) noexcept {
        switch (filter) {
            case TextureFilter::Linear:  return GL_LINEAR;
            case TextureFilter::Nearest: return GL_NEAREST;
        }

        MCT_ASSERT(false, "Invalid TexutureFilter type choosen!");
        return -1;
    }

    int32_t SwizzleToGLOrDefault(Swizzle swizzle, int32_t defaultSwizzle) noexcept {
        switch (swizzle) {
            case Swizzle::Red:   return GL_RED;
            case Swizzle::Green: return GL_GREEN;
            case Swizzle::Blue:  return GL_BLUE;
            case Swizzle::Alpha: return GL_ALPHA;
            case Swizzle::Zero:  return GL_ZERO;
            case Swizzle::One:   return GL_ONE;
            case Swizzle::None:  return defaultSwizzle;
        }

        return defaultSwizzle;
    }

    //_____________________________________________________________________________________________________________________________

}
