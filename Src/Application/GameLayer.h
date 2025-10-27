

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Layer.h"
#include "Player/Player.h"
#include "World/World.h"
#include "Renderer/GameRenderer.h"


namespace Mct {

    class GameLayer : public Layer {
    public:
        GameLayer();

        virtual void OnUpdate() override;

    private:
        Player m_Player;
        World  m_World;
        GameRenderer m_Renderer;
    };

}
