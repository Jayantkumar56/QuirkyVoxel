

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GameRenderer.h"
#include "WorldRenderer.h"
#include "Player/Player.h"
#include "Renderer/Primitives/FrameBuffer.h"
#include "Utils/Profiler.h"

#include <glad/glad.h>


namespace Mct {

    GameRenderer::GameRenderer() {
        FrameBufferSpec spec;
        spec.Width  = 1280;
        spec.Height = 720;

        m_FrameBuffer = std::make_unique<FrameBuffer>(spec);
        m_FrameBuffer->SetAttachments({
			{ FrameBufferTextureType::RGBA_8,             { .RGBA = {0.2f, 0.5f, 0.5f, 1.0f} } },
			{ FrameBufferTextureType::DEPTH_24_STENCIL_8, { .DepthValue = 1.0f }               }
		});

        m_WorldRenderer = std::make_unique<WorldRenderer>();

        // Enables back face culling.
        glEnable(GL_CULL_FACE);
    }

    GameRenderer::~GameRenderer() = default;

    void GameRenderer::Render(World& world, Player& player) {
        MCT_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();

        m_FrameBuffer->ClearAttachments();

        m_WorldRenderer->Render(player.GetCamera(), world);

        m_FrameBuffer->Unbind();
    }

    void GameRenderer::OnViewportResize(uint32_t width, uint32_t height) {
        glViewport(0, 0, width, height);
        m_FrameBuffer->Resize(width, height);
    }

    uint32_t GameRenderer::GetFinalTextureID() const noexcept {
        // Returns ID of the color texture.
        return m_FrameBuffer->GetColorAttachment(0);
    }

}
