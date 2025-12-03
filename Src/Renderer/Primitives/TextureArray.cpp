

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "TextureArray.h"
#include "Utils/Assert.h"
#include "Utils/TextureUtils.h"

#include <glad/glad.h>

#include <utility>


namespace Mct {
    
    TextureArray::TextureArray(std::span<const std::string> filepaths, TextureProperties properties) {
        MCT_ASSERT(!filepaths.empty(), "TextureArray: No filepaths provided!");

        const uint32_t channels = 4;                                     // RGBA (force channels)

        std::optional<Image> master;
        size_t masterIndex = 0;

        for (size_t i = 0; i < filepaths.size(); ++i) {
            master = Image::Create(filepaths[i], true, channels);
            if (master) {
                masterIndex = i;
                break;
            }
        }

        if (!master) {
            MCT_ERROR("TextureArray: Failed to load ANY valid images.");
            return;
        }

        const uint32_t    width    = master->GetWidth();
        const uint32_t    height   = master->GetHeight();
        const uint32_t    layers   = static_cast<uint32_t>(filepaths.size());
        const ImgDataType type     = master->GetDataType();

        const uint32_t bytesPerChannel = GetImgDataTypeSize(type);
        const size_t layerStride = static_cast<size_t>(width)    * static_cast<size_t>(height) *
                                   static_cast<size_t>(channels) * static_cast<size_t>(bytesPerChannel);

        std::vector<uint8_t> buffer;
        buffer.resize(layerStride * static_cast<size_t>(layers));

        uint8_t* bufferPtr = buffer.data();

        std::optional<Image> cachedFallback;

        for (size_t i = 0; i < filepaths.size(); ++i) {
            const std::optional<Image> optImg = (i == masterIndex) ? 
                                                std::move(master) : 
                                                Image::Create(filepaths[i], true, channels);

            bool isImageValid = false;
            if (optImg) {
                isImageValid = (optImg->GetWidth()    == width   ) &&
                               (optImg->GetHeight()   == height  ) &&
                               (optImg->GetDataType() == type    ) &&
                               (optImg->GetChannels() == channels);

                if (!isImageValid) {
                    MCT_ERROR("TextureArray: Mismatch in '{}'. Expected {}x{} C{} Type {}, got {}x{} C{} Type {}",
                        filepaths[i], width, height, channels, int(type),
                        optImg->GetWidth(), optImg->GetHeight(), optImg->GetChannels(), int(optImg->GetDataType()));
                }
            }
            else {
                MCT_ERROR("TextureArray: Failed to load '{}'", filepaths[i]);
            }

            const Image* imgToCopy = nullptr;

            if (isImageValid) {
                imgToCopy = &(*optImg);
            }
            else {
                // Lazy-init fallback
                if (!cachedFallback) {
                    cachedFallback = TextureUtils::GetMissingTexture(width, height, channels, type);
                }

                imgToCopy = &(*cachedFallback);
            }

            const uint8_t* rawBytes = static_cast<const uint8_t*>(imgToCopy->GetData());

            if (!rawBytes) {
                MCT_ERROR("TextureArray: Null pixel data for '{}'", filepaths[i]);
                std::memset(bufferPtr + layerStride * i, 0, layerStride);
            }
            else {
                std::memcpy(bufferPtr + layerStride * i, rawBytes, layerStride);
            }
        }

        Recreate(buffer.data(), type, width, height, channels, layers, properties);
    }

    TextureArray::~TextureArray() {
        if (m_RendererID) {
            glDeleteTextures(1, &m_RendererID);
        }
    }

    TextureArray::TextureArray(TextureArray&& other) noexcept :
            m_RendererID ( std::exchange(other.m_RendererID, 0) ),
            m_Width      ( std::exchange(other.m_Width,      0) ),
            m_Height     ( std::exchange(other.m_Height,     0) ),
            m_Layers     ( std::exchange(other.m_Layers,     0) ),
            m_Properties ( other.m_Properties                   )
    {}

    TextureArray& TextureArray::operator=(TextureArray&& other) noexcept {
        if (this != &other) {
            if (m_RendererID) {
                glDeleteTextures(1, &m_RendererID);
            }

            m_RendererID = std::exchange(other.m_RendererID, 0);
            m_Width      = std::exchange(other.m_Width,      0);
            m_Height     = std::exchange(other.m_Height,     0);
            m_Layers     = std::exchange(other.m_Layers,     0);
            m_Properties = other.m_Properties;
        }

        return *this;
    }

    void TextureArray::Bind(uint32_t textureUnit) const noexcept {
        glBindTextureUnit(textureUnit, m_RendererID);
    }

    void TextureArray::Recreate(const void*       data,
                                ImgDataType       dataType,
                                uint32_t          width,
                                uint32_t          height,
                                uint32_t          channels,
                                uint32_t          layers,
                                TextureProperties properties) noexcept
    {
        if (!data) {
            MCT_ERROR("Data not provided for Texture Array creation.");
            return;
        }

        if (m_RendererID) {
            glDeleteTextures(1, &m_RendererID);
        }

        m_Width      = width;
        m_Height     = height;
        m_Layers     = layers;
        m_Properties = properties;

        const ImgDataFmt     dataFormat     = ImgDataFmtFromChannels(channels);
        const ImgInternalFmt internalFormat = properties.InternalFormat != ImgInternalFmt::None ?
                                              properties.InternalFormat                         :
                                              ImgInternalFmtFromChannels(channels);

        const GLenum glDataFormat     = ImgDataFmtToGL(dataFormat);
        const GLenum glInternalFormat = ImgInternalFmtToGL(internalFormat);

        const GLsizei maxPossibleLevels = static_cast<GLsizei>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;
        const GLsizei requestedLevels   = (properties.MipLevels == 0) ?
                                            maxPossibleLevels :
                                            static_cast<GLsizei>(properties.MipLevels);

        // If MipLevels is 0, use max. Otherwise, clamp user input to max.
        const GLsizei mipLevels = std::min(requestedLevels, maxPossibleLevels);

        const GLenum glUploadType = ImgDataTypeToGL(dataType);
        
        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID);
        glTextureStorage3D(m_RendererID, mipLevels, glInternalFormat, width, height, layers);

        if (dataFormat != ImgDataFmt::RGBA) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }

        glTextureSubImage3D(
            m_RendererID, 
            0, 0, 0, 0,
            width,
            height,
            layers,
            glDataFormat,
            glUploadType,
            data
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

}
 