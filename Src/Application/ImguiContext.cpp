

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "ImguiContext.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/EventDispatcher.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


namespace Mct {

	void ImguiContext::Init(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();

        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, 
        // changing this requires resetting Style + calling this again)
        style.ScaleAllSizes(mainScale);
        style.FontScaleDpi = mainScale;

        const char* glsl_version = "#version 460 core";
        ImGui_ImplGlfw_InitForOpenGL(window, false);
        ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImguiContext::Terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

	void ImguiContext::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	}

	void ImguiContext::End() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
	}

    std::unique_ptr<WindowCallbacks> ImguiContext::GetEventCallbacks() {
        std::unique_ptr<WindowCallbacks> eventCallbacks = std::make_unique<WindowCallbacks>();

        eventCallbacks->Key         = ImGui_ImplGlfw_KeyCallback;
        eventCallbacks->Char        = ImGui_ImplGlfw_CharCallback;
        eventCallbacks->MouseButton = ImGui_ImplGlfw_MouseButtonCallback;
        eventCallbacks->CursorPos   = ImGui_ImplGlfw_CursorPosCallback;
        eventCallbacks->Scroll      = ImGui_ImplGlfw_ScrollCallback;

        return eventCallbacks;
    }

}
