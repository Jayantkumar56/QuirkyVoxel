

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "TextureArray.h"
#include "Utils/Image.h"
#include "Common/Assert.h"

#include <glad/glad.h>


namespace Mct {

    TextureArray::TextureArray(std::span<const std::string> filepaths) {
        MCT_ASSERT(!filepaths.empty() && "No filepaths provided");

        m_Layers = static_cast<uint32_t>(filepaths.size());

        Image firstImage(filepaths[0], true, 4);

        MCT_ASSERT(firstImage.IsValid() && "Failed to load first image");

        m_Width  = static_cast<uint32_t>(firstImage.GetWidth());
        m_Height = static_cast<uint32_t>(firstImage.GetHeight());

        // Create OpenGL Texture and Allocate Storage
        {
            glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID);

            // Allocate immutable storage for all layers (1 mipmap level)
            glTextureStorage3D(
                m_RendererID,
                1,             // Mipmap levels
                GL_RGBA8,      // Internal format
                m_Width,       // Width of each slice
                m_Height,      // Height of each slice
                m_Layers       // Number of slices
            );
        }

        // Upload the First Image's Data
        glTextureSubImage3D(
            m_RendererID,
            0,                   // Mipmap level
            0, 0, 0,             // xoffset, yoffset, zoffset (slice index)
            m_Width,             // width
            m_Height,            // height
            1,                   // depth (1 slice)
            GL_RGBA,             // Format of the pixel data (matches our 4 channels)
            GL_UNSIGNED_BYTE,    // Type of the pixel data
            firstImage.GetData() // The image data
        );

        for (uint32_t i = 1; i < m_Layers; ++i) {
            Image image(filepaths[i], true, 4);

            MCT_ASSERT(image.IsValid() && "Failed to load image");

            MCT_ASSERT(image.GetWidth() == m_Width || image.GetHeight() == m_Height && 
                "Images in a Texture Array must have same dimension.");

            glTextureSubImage3D(
                m_RendererID,
                0,                 // Mipmap level
                0, 0, i,           // xoffset, yoffset, zoffset (slice index)
                m_Width,           // width
                m_Height,          // height
                1,                 // depth (1 slice)
                GL_RGBA,           // Format
                GL_UNSIGNED_BYTE,  // Type
                image.GetData()    // The image data
            );
        }

        // TODO: Make these configurable
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    TextureArray::~TextureArray() {
        glDeleteTextures(1, &m_RendererID);
    }

    TextureArray::TextureArray(TextureArray&& other) noexcept : 
            m_RendererID ( other.m_RendererID ),
            m_Width      ( other.m_Width      ),
            m_Height     ( other.m_Height     ),
            m_Layers     ( other.m_Layers     ) {
        other.m_RendererID = 0;
        other.m_Width      = 0;
        other.m_Height     = 0;
        other.m_Layers     = 0;
    }

    TextureArray& TextureArray::operator=(TextureArray&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &m_RendererID);

            m_RendererID = other.m_RendererID;
            m_Width      = other.m_Width;
            m_Height     = other.m_Height;
            m_Layers     = other.m_Layers;

            other.m_RendererID = 0;
            other.m_Width      = 0;
            other.m_Height     = 0;
            other.m_Layers     = 0;
        }

        return *this;
    }

    void TextureArray::GenerateMipmaps() const noexcept {
        glGenerateTextureMipmap(m_RendererID);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    }

    void TextureArray::Bind(uint32_t textureUnit) const noexcept {
        glBindTextureUnit(textureUnit, m_RendererID);
    }

    void TextureArray::Unbind(uint32_t textureUnit) const noexcept {
        glBindTextureUnit(textureUnit, 0);
    }

}
