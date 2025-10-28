

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Common/NonCopyable.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string_view>


namespace Mct {

	class Window : public NonCopyable {
	public:
		[[nodiscard]] static bool Initialise() noexcept;
		static void Terminate() noexcept;

		[[nodiscard]] static std::optional<Window> Create(const std::string_view title,
														  const int width,
														  const int height) noexcept;
	public:
		~Window();

		Window(Window&& other) noexcept : m_WindowHandle(other.m_WindowHandle) {
			other.m_WindowHandle = nullptr;
		}

		Window& operator=(Window&& other) noexcept {
			if (this != &other) {
				if (m_WindowHandle) {
					glfwDestroyWindow(m_WindowHandle);
				}

				m_WindowHandle = other.m_WindowHandle;
				other.m_WindowHandle = nullptr;
			}

			return *this;
		}

		[[nodiscard]] GLFWwindow* GetHandle() const noexcept {
			return m_WindowHandle;
		}

		[[nodiscard]] bool ShouldClose() const noexcept {
			return glfwWindowShouldClose(m_WindowHandle);
		}

		void PollEvents() const noexcept {
			glfwPollEvents();
		}

		void SwapBuffers() const noexcept {
			glfwSwapBuffers(m_WindowHandle);
		}

	private:
		[[nodiscard]] static bool InitialiseGlad() noexcept;

		Window(GLFWwindow* windowHandle) noexcept : m_WindowHandle(windowHandle) {}

	private:
		static bool s_GLFWInitialised;
		static bool s_GladInitialized;

	private:
		GLFWwindow* m_WindowHandle;
	};

}
