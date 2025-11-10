

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "Window.h"
#include "World/Block/BlockDataManager.h"

#include <iostream>


int main() {
    // Initialise the global GLFW state first.
    if (!Mct::Window::Initialise()) {
        std::cerr << "Failed to initialise GLFW!" << std::endl;
        return -1;
    }

    Mct::BlockDataManager::Init();

    {
        Mct::Application app;
        app.Run();
    }

    // Clean up GLFW
    Mct::Window::Terminate();

    return 0;
}
