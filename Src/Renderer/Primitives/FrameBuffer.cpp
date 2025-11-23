

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "FrameBuffer.h"
#include "Utils/Assert.h"

#include <glad/glad.h>

#include <array>
#include <numeric>


namespace Mct {

    static constexpr int TextureWrapToOpenglWrap(TextureWrap wrap) noexcept {
        switch (wrap) {
            case TextureWrap::Repeat:         return GL_REPEAT;
            case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
            case TextureWrap::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        }

        MCT_ASSERT(false && "Invalid TextureWrap type choosen!");
        return -1;
    }

    static constexpr int TexutureFilterToOpenglFilter(TextureFilter filter) noexcept {
        switch (filter) {
            case TextureFilter::Linear:  return GL_LINEAR;
            case TextureFilter::Nearest: return GL_NEAREST;
        }

        MCT_ASSERT(false && "Invalid TexutureFilter type choosen!");
        return -1;
    }

    static constexpr bool IsFrameBufferColorAttachment(FrameBufferTextureType type) noexcept {
        return type == FrameBufferTextureType::RGBA_8            ||
               type == FrameBufferTextureType::RGB_8             ||
               type == FrameBufferTextureType::RED_INTEGER;
    }

    static constexpr bool IsFrameBufferDepthAttachment(FrameBufferTextureType type) noexcept {
        return type == FrameBufferTextureType::DEPTH_24_STENCIL_8;
    }

    FrameBuffer::FrameBuffer(const FrameBufferSpec& spec) noexcept :
            m_FrameBufferSpec(spec)
    {
        glGenFramebuffers(1, &m_RendererID);
    }

    FrameBuffer::~FrameBuffer() noexcept {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void FrameBuffer::Bind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void FrameBuffer::Unbind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::ClearAttachments() const noexcept {
        if (m_DepthStencilAttachment)
            glClear(GL_DEPTH_BUFFER_BIT);

        for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
            const auto& colorAttachment = m_ColorAttachmentsSpec[i];

            switch (colorAttachment.Type) {
                case FrameBufferTextureType::RGBA_8: {
                    glClearBufferfv(GL_COLOR, i, colorAttachment.ClearData.RGBA); break;
                }

                case FrameBufferTextureType::RGB_8: {
                    glClearBufferfv(GL_COLOR, i, colorAttachment.ClearData.RGBA); break;
                }

                case FrameBufferTextureType::RED_INTEGER: {
                    glClearBufferiv(GL_COLOR, i, &(colorAttachment.ClearData.RedInteger)); break;
                }
            }
        }
    }

    void FrameBuffer::SetAttachments(std::initializer_list<FrameBufferAttachmentSpec> attachmentsSpec) {
        int colorAttachmentCount = 0;

        for (auto attachmentSpec : attachmentsSpec) {
            if (IsFrameBufferColorAttachment(attachmentSpec.Type)) {
                ++colorAttachmentCount;
                m_ColorAttachmentsSpec.emplace_back(attachmentSpec);
            }
            else if (IsFrameBufferDepthAttachment(attachmentSpec.Type)) {
                MCT_ASSERT(
                    !m_DepthAttachmentSpec &&
                    "Framebuffer do not support multiple DepthAttachments"
                );

                m_DepthAttachmentSpec = attachmentSpec;
            }
        }

        MCT_ASSERT(colorAttachmentCount < 32 && "Provided ColorAttachments more than max limit of 32 attachments");
    }     

    uint32_t FrameBuffer::GetDepthStencilAttachment() const noexcept {
        MCT_ASSERT(m_DepthStencilAttachment);
        return *m_DepthStencilAttachment;
    }

    uint32_t FrameBuffer::GetColorAttachment(size_t index) const noexcept {
        MCT_ASSERT(index < m_ColorAttachments.size() && "Index out of bound for color attachment");
        return m_ColorAttachments[index];
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0)
            return;

        m_FrameBufferSpec.Width  = width;
        m_FrameBufferSpec.Height = height;

        InvalidateAttachments();
        CreateAttachments();
    }

    uint32_t FrameBuffer::CreateBuffer(int          internalFormat,
                                       unsigned int format,
                                       unsigned int dataType,
                                       FrameBufferAttachmentSpec spec) const 
    {
        uint32_t buffer;

        glCreateTextures(GL_TEXTURE_2D, 1, &buffer);
        glBindTexture(GL_TEXTURE_2D, buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_FrameBufferSpec.Width, m_FrameBufferSpec.Height, 0, format, dataType, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexutureFilterToOpenglFilter(spec.MinificationFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexutureFilterToOpenglFilter(spec.MagficationFilter));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToOpenglWrap(spec.WrapMode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToOpenglWrap(spec.WrapMode));

        return buffer;
    }

    void FrameBuffer::CreateAttachments() {
        static const std::array colorAttachments = []() noexcept {
            std::array<GLenum, 32> attachments{};
            std::iota(attachments.begin(), attachments.end(), GL_COLOR_ATTACHMENT0);
            return attachments;
        }();

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        m_ColorAttachments.resize(m_ColorAttachmentsSpec.size());

        for (size_t i = 0; i < m_ColorAttachmentsSpec.size(); ++i) {
            switch (m_ColorAttachmentsSpec[i].Type) {
                case FrameBufferTextureType::RGBA_8: {
                    m_ColorAttachments[i] = CreateBuffer(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m_ColorAttachmentsSpec[i]);
                    break;
                }

                case FrameBufferTextureType::RGB_8: {
                    m_ColorAttachments[i] = CreateBuffer(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, m_ColorAttachmentsSpec[i]);
                    break;
                }

                case FrameBufferTextureType::RED_INTEGER: {
                    m_ColorAttachments[i] = CreateBuffer(GL_R32I, GL_RED_INTEGER, GL_INT, m_ColorAttachmentsSpec[i]);
                    break;
                }
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[i], GL_TEXTURE_2D, m_ColorAttachments[i], 0);
        }

        if (m_DepthAttachmentSpec) {
            uint32_t depthStencilAttachment = 0;

            switch (m_DepthAttachmentSpec->Type) {
                case FrameBufferTextureType::DEPTH_24_STENCIL_8: {
                    depthStencilAttachment = CreateBuffer(GL_DEPTH24_STENCIL8,
                                                          GL_DEPTH_STENCIL, 
                                                          GL_UNSIGNED_INT_24_8, 
                                                          *m_DepthAttachmentSpec);

                    glFramebufferTexture2D(GL_FRAMEBUFFER, 
                                           GL_DEPTH_STENCIL_ATTACHMENT, 
                                           GL_TEXTURE_2D,
                                           depthStencilAttachment,
                                           0);
                    break;
                }
            }

            m_DepthStencilAttachment = depthStencilAttachment;
        }
        
        const GLint colorBufferCount = static_cast<GLint>(m_ColorAttachments.size());
        glDrawBuffers(colorBufferCount, colorAttachments.data());

        MCT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Uncomplete FrameBuffer!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::InvalidateAttachments() {
        if (!m_ColorAttachments.empty()) {
            glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
            m_ColorAttachments.clear();
        }

        if (m_DepthStencilAttachment) {
            const uint32_t depthStencilAttachment = *m_DepthStencilAttachment;
            glDeleteTextures(1, &depthStencilAttachment);
            m_DepthStencilAttachment = std::nullopt;
        }
    }

}
