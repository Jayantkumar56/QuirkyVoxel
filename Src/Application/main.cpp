

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "Window.h"
#include "Utils/Logger.h"
#include "Utils/Assert.h"


int main() {
    Mct::Log::Init();

    {
        const bool windowInitialised = Mct::Window::Initialise();
        MCT_ASSERT(windowInitialised, "Failed to initialise Window!");
    }

    {
        Mct::Application app;
        app.Run();
    }

    Mct::Window::Terminate();

    Mct::Log::Shutdown();

    return 0;
}
