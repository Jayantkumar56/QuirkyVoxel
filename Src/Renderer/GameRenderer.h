

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <memory>


namespace Mct {

    class World;
    class Player;

    class WorldRenderer;

    class GameRenderer {
    public:
        GameRenderer();
        ~GameRenderer();

        void Render(World& world, Player& player);

    private:
        std::unique_ptr<WorldRenderer> m_WorldRenderer;
    };

}
