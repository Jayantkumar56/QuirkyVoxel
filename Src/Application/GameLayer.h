

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

        // IMPORTANT
        // m_Renderer MUST be declared before m_World.
        //
        // This controls the C++ destruction order. The MeshManager (owned by m_Renderer) 
        // should be destroyed after the ChunkManager (owned by m_World).
        //
        // If m_World is destroyed first, its worker threads can deadlock trying to access 
        // the MeshManager's deletion queue while the main thread is already destroying it.

        GameRenderer m_Renderer;
        World        m_World;

        glm::vec2 m_GameViewportSize = { 0.0f, 0.0f };
    };

}
