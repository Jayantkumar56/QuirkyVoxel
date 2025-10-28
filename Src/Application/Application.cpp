

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Application.h"
#include "GameLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>


namespace Mct {

    Application::Application() noexcept {
        // Initialise the global GLFW state first.
        if (!Window::Initialise()) {
            std::cerr << "Failed to initialise GLFW!" << std::endl;
            return;
        }

        m_Window = Window::Create("Minecraft Clone", 1280, 720);
        InitImgui();
    }

    Application::~Application() {
        Window::Terminate(); // Clean up GLFW
        TerminateImgui();
    }

    void Application::Run() {
        // Run the loop only when the window was successfully created.
        if (!m_Window) {
            return;
        }

        PushLayer<GameLayer>();

        while (!m_Window->ShouldClose()) {
            m_Window->PollEvents();

            glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            BeginImgui();

            {
                ImGui::Begin("Hello, world!");
                ImGuiIO& io = ImGui::GetIO();

                ImGui::Text("This is some useful text.");

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            for (const auto& layer : m_LayerStack) {
                layer->OnUpdate();
            }

            EndImgui();

            m_Window->SwapBuffers();
        }
    }

    void Application::InitImgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, 
        // changing this requires resetting Style + calling this again)
        style.ScaleAllSizes(mainScale);
        style.FontScaleDpi = mainScale;

        const char* glsl_version = "#version 450 core";
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Application::TerminateImgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::BeginImgui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::EndImgui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
