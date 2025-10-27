

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "GameRenderer.h"
#include "WorldRenderer.h"
#include "Player/Player.h"


namespace Mct {

    GameRenderer::GameRenderer()  = default;
    GameRenderer::~GameRenderer() = default;

    void GameRenderer::Render(World& world, Player& player) {
        m_WorldRenderer->Render(player.GetCamera(), world);
    }

}
