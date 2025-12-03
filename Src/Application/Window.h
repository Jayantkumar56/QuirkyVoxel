

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#pragma once


#include "Utils/NonCopyable.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <memory>


namespace Mct {

	struct WindowCallbacks {
		// TODO: Implement these remaining callbacks
		// 
		//GLFWerrorfun                         = nullptr;
		//GLFWwindowposfun                     = nullptr;
		//GLFWwindowrefreshfun                 = nullptr;
		//GLFWwindowfocusfun                   = nullptr;
		//GLFWwindowiconifyfun                 = nullptr;
		//GLFWwindowmaximizefun                = nullptr;
		//GLFWframebuffersizefun               = nullptr;
		//GLFWwindowcontentscalefun            = nullptr;
		//GLFWcursorenterfun                   = nullptr;
		//GLFWcharmodsfun                      = nullptr;
		//GLFWdropfun                          = nullptr;
		//GLFWjoyfun                           = nullptr;
		//GLFWmonitorfun                       = nullptr;

		GLFWwindowsizefun             WindowSize   = nullptr;
		GLFWwindowclosefun            WindowClose  = nullptr;
		GLFWkeyfun                    Key          = nullptr;
		GLFWcharfun                   Char         = nullptr;
		GLFWmousebuttonfun            MouseButton  = nullptr;
		GLFWcursorposfun              CursorPos    = nullptr;
		GLFWscrollfun                 Scroll       = nullptr;
	};

	class Window : public NonCopyable {
	public:
		[[nodiscard]] static bool Initialise() noexcept;
		static void Terminate() noexcept;

		[[nodiscard]] static std::unique_ptr<Window> Create(const std::string_view title,
														    const int width,
														    const int height,
														    std::unique_ptr<WindowCallbacks> eventCallbacks = nullptr) noexcept;
	public:
		~Window();

		Window(Window&& other) noexcept : 
				m_Width          ( other.m_Width                     ),
				m_Height         ( other.m_Height                    ),
				m_WindowHandle   ( other.m_WindowHandle              ),
				m_MouseState     ( other.m_MouseState                ),
				m_EventCallbacks ( std::move(other.m_EventCallbacks) )
		{
			other.m_WindowHandle = nullptr;
		}

		Window& operator=(Window&& other) noexcept {
			if (this != &other) {
				if (m_WindowHandle) {
					glfwDestroyWindow(m_WindowHandle);
				}

				m_Width          =  other.m_Width;
				m_Height         =  other.m_Height;
				m_WindowHandle   = other.m_WindowHandle;
				m_MouseState     = other.m_MouseState;
				m_EventCallbacks = std::move(other.m_EventCallbacks);

				other.m_WindowHandle = nullptr;
			}

			return *this;
		}

		[[nodiscard]] int GetWidth()  const noexcept { return m_Width;  }
		[[nodiscard]] int GetHeight() const noexcept { return m_Height; }

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

		void MakeContextCurrent() const noexcept {
			glfwMakeContextCurrent(m_WindowHandle);
		}

		void SetCursorMode(int mode) noexcept {
			glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, mode);
		}

	private:
		struct MouseState {
			double LastX   = 0.0;
			double LastY   = 0.0;
			bool   HasLast = false;
		};

	private:
		[[nodiscard]] static bool InitialiseGlad() noexcept;

		Window(GLFWwindow* windowHandle, std::unique_ptr<WindowCallbacks> eventCallbacks, int width, int height) noexcept :
				m_Width          ( width                     ),
				m_Height         ( height                    ),
				m_WindowHandle   ( windowHandle              ),
				m_EventCallbacks ( std::move(eventCallbacks) )
		{}

		void SetKeyCallback();
		void SetCharCallback();
		void SetMouseButtonCallback();
		void SetScrollCallback();
		void SetCursorPosCallback();
		void SetWindowSizeCallback();
		void SetWindowCloseCallback();

	private:
		static bool s_GLFWInitialised;
		static bool s_GladInitialized;

	private:
		int m_Width;
		int m_Height;

		GLFWwindow* m_WindowHandle;
		MouseState  m_MouseState;
		std::unique_ptr<WindowCallbacks> m_EventCallbacks;
	};

}
