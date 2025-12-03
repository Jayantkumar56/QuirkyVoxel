

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Texture2D.h"

#include <vector>
#include <optional>


namespace Mct {

    enum class FrameBufferTextureType : uint8_t {
        None = 0,

        // color attachments
        RGBA_8,
        RGB_8,
        RED_INTEGER,

        // depth and stencile attachments
        DEPTH_24_STENCIL_8
    };

    union ClearAttachmentData {
        float RGBA[4];
        int	  RedInteger;
        float DepthValue;
    };

    struct FrameBufferAttachmentSpec {
        FrameBufferTextureType Type = FrameBufferTextureType::None;
        ClearAttachmentData    ClearData = { 0.0f, 0.0f, 0.0f, 1.0f };
        TextureWrap			   WrapMode  = TextureWrap::Repeat;
        TextureFilter MinificationFilter = TextureFilter::Linear;
        TextureFilter MagficationFilter  = TextureFilter::Linear;
    };

    struct FrameBufferSpec {
        uint32_t Width = 0, Height = 0;
    };

    class FrameBuffer {
    public:
        FrameBuffer(const FrameBufferSpec& spec) noexcept;
        ~FrameBuffer() noexcept;

        void Bind()   const noexcept;
        void Unbind() const noexcept;

        void ClearAttachments() const noexcept;
        void SetAttachments(std::initializer_list<FrameBufferAttachmentSpec> attachmentsSpec);

        uint32_t GetDepthStencilAttachment()      const noexcept;
        uint32_t GetColorAttachment(size_t index) const noexcept;

        void Resize(uint32_t width, uint32_t height);

    private:
        uint32_t CreateBuffer(int          internalFormat, 
                              unsigned int format, 
                              unsigned int dataType, 
                              FrameBufferAttachmentSpec spec) const;

        void CreateAttachments();
        void InvalidateAttachments();

    private:
        uint32_t m_RendererID;
        std::optional<uint32_t> m_DepthStencilAttachment;
        std::vector<uint32_t>   m_ColorAttachments;

        FrameBufferSpec                          m_FrameBufferSpec;
        std::optional<FrameBufferAttachmentSpec> m_DepthAttachmentSpec;
        std::vector<FrameBufferAttachmentSpec>   m_ColorAttachmentsSpec;
    };

}
