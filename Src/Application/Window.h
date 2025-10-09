

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <optional>
#include <string_view>


class Window {
public:
	[[nodiscard]] static bool Initialise() noexcept;
	static void Terminate() noexcept;

	[[nodiscard]] static std::optional<Window> Create(const std::string_view title,
		                                              const int width,
		                                              const int height) noexcept;
public:
	~Window();

	// Non-copyable
	Window(const Window&)            = delete;
	Window& operator=(const Window&) = delete;

	inline Window(Window&& other) noexcept : m_WindowHandle(other.m_WindowHandle) {
		other.m_WindowHandle = nullptr;
	}

	inline Window& operator=(Window&& other) noexcept {
		if (this != &other) {
			if (m_WindowHandle) {
				glfwDestroyWindow(m_WindowHandle);
			}

			m_WindowHandle = other.m_WindowHandle;
			other.m_WindowHandle = nullptr;
		}

		return *this;
	}

	[[nodiscard]] inline bool ShouldClose() const noexcept {
		return glfwWindowShouldClose(m_WindowHandle);
	}

	inline void PollEvents() const noexcept {
		glfwPollEvents();
	}

	inline void SwapBuffers() const noexcept {
		glfwSwapBuffers(m_WindowHandle);
	}

private:
	[[nodiscard]] static bool InitialiseGlad() noexcept;

	inline Window(GLFWwindow* windowHandle) noexcept : m_WindowHandle(windowHandle) {}

private:
	static bool s_GLFWInitialised;
	static bool s_GladInitialized;

private:
	GLFWwindow* m_WindowHandle;
};
