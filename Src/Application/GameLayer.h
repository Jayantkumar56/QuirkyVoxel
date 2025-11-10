

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Layer.h"
#include "Player/Player.h"
#include "World/World.h"
#include "Renderer/GameRenderer.h"


namespace Mct {

    class GameLayer final : public Layer {
    public:
        GameLayer();

        virtual void OnAttach()                override;
        virtual void OnEvent(Event& e)         override;
        virtual void OnUpdate(float deltaTime) override;

    private:
        void UpdateGameUI(float deltaTime);
        void UpdateDebugUI(float deltaTime);

    private:
        bool   m_GameHaveFocus = false;
        Player m_Player;
        World  m_World;
        GameRenderer m_Renderer;

        glm::vec2 m_GameViewportSize = { 0.0f, 0.0f };
    };

}
