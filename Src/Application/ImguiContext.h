

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Window.h"


namespace Mct {

	class Event;

	class ImguiContext {
	public:
		static void Init(GLFWwindow* window);
		static void Terminate();

		static void Begin();
		static void End();

		static std::unique_ptr<WindowCallbacks> GetEventCallbacks();
	};

}
