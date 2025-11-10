

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <memory>


namespace Mct {

    class FrameBuffer;
    class WorldRenderer;

    class World;
    class Player;

    class GameRenderer {
    public:
        GameRenderer();
        ~GameRenderer();

        void Render(World& world, Player& player);

        void OnViewportResize(uint32_t width, uint32_t height);

        uint32_t GetFinalTextureID() const noexcept;

    private:
        std::unique_ptr<FrameBuffer>   m_FrameBuffer;
        std::unique_ptr<WorldRenderer> m_WorldRenderer;
    };

}
