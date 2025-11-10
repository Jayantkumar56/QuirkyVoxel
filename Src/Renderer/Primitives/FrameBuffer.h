

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <vector>


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

    inline bool IsFrameBufferColorAttachment(FrameBufferTextureType type) noexcept {
        bool isColorType = type == FrameBufferTextureType::RGBA_8;
        isColorType = isColorType || (type == FrameBufferTextureType::RGB_8);
        isColorType = isColorType || (type == FrameBufferTextureType::RED_INTEGER);
        return isColorType;
    }

    inline bool IsFrameBufferDepthAttachment(FrameBufferTextureType type) noexcept {
        bool isDepthType = type == FrameBufferTextureType::DEPTH_24_STENCIL_8;
        return isDepthType;
    }

    union ClearAttachmentData {
        float RGBA[4];
        int	  RedInteger;
        float DepthValue;
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

    struct FrameBufferAttachmentSpecification {
        FrameBufferTextureType Type = FrameBufferTextureType::None;
        ClearAttachmentData    ClearData = { 0.0f, 0.0f, 0.0f, 1.0f };
        TextureWrap			   WrapMode  = TextureWrap::Repeat;
        TextureFilter MinificationFilter = TextureFilter::Linear;
        TextureFilter MagficationFilter  = TextureFilter::Linear;
    };

    struct FrameBufferSpecification {
        uint32_t Width = 0, Height = 0;
    };

    class FrameBuffer {
    public:
        FrameBuffer(const FrameBufferSpecification& spec) noexcept;
        ~FrameBuffer() noexcept;

        void Bind()   noexcept;
        void Unbind() noexcept;

        void ClearAttachments();
        void SetAttachments(std::initializer_list<FrameBufferAttachmentSpecification> attachmentsSpec);

        uint32_t GetDepthStencilAttachment()      const noexcept;
        uint32_t GetColorAttachment(size_t index) const noexcept;

        void Resize(uint32_t width, uint32_t height);
        void GetColorPixelData(size_t index, int x, int y, int width, int height, void* outputData, int size);

    private:
        uint32_t CreateBuffer(int          internalFormat, 
                              unsigned int format, 
                              unsigned int dataType, 
                              FrameBufferAttachmentSpecification spec) const;

        void CreateAttachments();
        void InvalidateAttachments();

    private:
        uint32_t m_RendererID;
        uint32_t m_DepthStencilAttachment = 0;
        std::vector<uint32_t> m_ColorAttachments;

        FrameBufferSpecification                        m_FrameBufferSpec;
        FrameBufferAttachmentSpecification              m_DepthAttachmentSpec;
        std::vector<FrameBufferAttachmentSpecification> m_ColorAttachmentsSpec;
    };

}
