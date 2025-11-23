

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Window.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/EventDispatcher.h"

#include <iostream>


namespace Mct {

	bool Window::s_GLFWInitialised = false;
	bool Window::s_GladInitialized = false;


	bool Window::Initialise() noexcept {
		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLFW" << std::endl;
			s_GLFWInitialised = false;
			return false;
		}

		s_GLFWInitialised = true;
		return true;
	}

	void Window::Terminate() noexcept {
		if (s_GLFWInitialised) {
			glfwTerminate();
		}
	}

	std::unique_ptr<Window> Window::Create(const std::string_view title,
									       const int width,
									       const int height,
										   std::unique_ptr<WindowCallbacks> eventCallbacks) noexcept
	{
		if (!s_GLFWInitialised) {
			std::cerr << "GLFW not initialised." << std::endl;
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		GLFWwindow* windowHandle = glfwCreateWindow(width, height, title.data(), NULL, NULL);

		if (windowHandle == NULL) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return nullptr;
		}

		glfwMakeContextCurrent(windowHandle);

		if (!s_GladInitialized) {
			if (!InitialiseGlad())
				return nullptr;

			static auto MessageCallback = +[](GLenum source, 
											  GLenum type, 
											  GLuint id, 
											  GLenum severity, 
											  GLsizei length, 
											  const GLchar* message, 
											  const void* userParam) noexcept 
			{
				std::cout << "[OpenGL Error](" << type << ") " << message << "\n";
			};

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(MessageCallback, 0);
		}

		std::unique_ptr<Window> window (new Window(windowHandle, std::move(eventCallbacks), width, height));
		glfwSetWindowUserPointer(windowHandle, &(*window));

		window->SetKeyCallback();
		window->SetCharCallback();
		window->SetMouseButtonCallback();
		window->SetScrollCallback();
		window->SetCursorPosCallback();
		window->SetWindowSizeCallback();
		window->SetWindowCloseCallback();

		return window;
	}

	Window::~Window() {
		if (m_WindowHandle) {
			glfwDestroyWindow(m_WindowHandle);
			m_WindowHandle = nullptr;
		}
	}

	bool Window::InitialiseGlad() noexcept {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			s_GladInitialized = false;
			return false;
		}

		s_GladInitialized = true;
		return true;
	}

	void Window::SetKeyCallback() {
		GLFWkeyfun callback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
			auto* wind = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->Key)
				wind->m_EventCallbacks->Key(window, key, scancode, action, mods);

			if (action == GLFW_PRESS) {
				KeyPressedEvent e{ key, mods };
				EventDispatcher::DispatchEvent(e);
			}
			else if (action == GLFW_REPEAT) {
				KeyRepeatEvent e{ key, mods };
				EventDispatcher::DispatchEvent(e);
			}
			else if (action == GLFW_RELEASE) {
				KeyReleasedEvent e{ key, mods };
				EventDispatcher::DispatchEvent(e);
			}
		};

		glfwSetKeyCallback(m_WindowHandle, callback);
	}

	void Window::SetCharCallback() {
		GLFWcharfun callback = [] (GLFWwindow* window, unsigned int codepoint) {
			auto* wind = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->Char)
				wind->m_EventCallbacks->Char(window, codepoint);

			// TODO: Implement this Event.
			//KeyTypedEvent e{ c };
			//EventDispatcher::DispatchEvent(e);
		};

		glfwSetCharCallback(m_WindowHandle, callback);
	}

	void Window::SetMouseButtonCallback() {
		GLFWmousebuttonfun callback = [] (GLFWwindow* window, int button, int action, int mods) {
			auto* wind       = static_cast<Window*>(glfwGetWindowUserPointer(window));
			auto& mouseState = wind->m_MouseState;

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->MouseButton)
				wind->m_EventCallbacks->MouseButton(window, button, action, mods);

			if (action == GLFW_PRESS) {
				MouseButtonPressedEvent e{ button, mods, mouseState.LastX, mouseState.LastY };
				EventDispatcher::DispatchEvent(e);
			}
			else if (action == GLFW_RELEASE) {
				MouseButtonReleasedEvent e{ button, mods, mouseState.LastX, mouseState.LastY };
				EventDispatcher::DispatchEvent(e);
			}
		};

		glfwSetMouseButtonCallback(m_WindowHandle, callback);
	}

	void Window::SetScrollCallback() {
		GLFWscrollfun callback = [] (GLFWwindow* window, double xoffset, double yoffset) {
			auto* wind       = static_cast<Window*>(glfwGetWindowUserPointer(window));
			auto& mouseState = wind->m_MouseState;

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->Scroll)
				wind->m_EventCallbacks->Scroll(window, xoffset, yoffset);

			MouseScrolledEvent e{ xoffset, yoffset, mouseState.LastX, mouseState.LastY };
			EventDispatcher::DispatchEvent(e);
		};

		glfwSetScrollCallback(m_WindowHandle, callback);
	}

	void Window::SetCursorPosCallback() {
		GLFWcursorposfun callback = [] (GLFWwindow* window, double xpos, double ypos) {
			auto* wind       = static_cast<Window*>(glfwGetWindowUserPointer(window));
			auto& mouseState = wind->m_MouseState;

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->CursorPos) {
				wind->m_EventCallbacks->CursorPos(window, xpos, ypos);
			}

			if (!mouseState.HasLast) {
				mouseState.LastX   = xpos;
				mouseState.LastY   = ypos;
				mouseState.HasLast = true;
			}

			const double prevX = mouseState.LastX;
			const double prevY = mouseState.LastY;
			const double curX  = xpos;
			const double curY  = ypos;

			MouseMovedEvent e{ prevX, prevY, curX, curY };
			EventDispatcher::DispatchEvent(e);

			mouseState.LastX = xpos;
			mouseState.LastY = ypos;
		};

		glfwSetCursorPosCallback(m_WindowHandle, callback);
	}

	void Window::SetWindowSizeCallback() {
		GLFWwindowsizefun callback = [] (GLFWwindow* window, int width, int height) {
			auto* wind = static_cast<Window*>(glfwGetWindowUserPointer(window));

			wind->m_Width  = width;
			wind->m_Height = height;

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->WindowSize)
				wind->m_EventCallbacks->WindowSize(window, width, height);

			WindowResizeEvent e{ width, height };
			EventDispatcher::DispatchEvent(e);
		};

		glfwSetWindowSizeCallback(m_WindowHandle, callback);
	}

	void Window::SetWindowCloseCallback() {
		GLFWwindowclosefun callback = [] (GLFWwindow* window) {
			auto* wind = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->WindowClose)
				wind->m_EventCallbacks->WindowClose(window);

			WindowCloseEvent e{};
			EventDispatcher::DispatchEvent(e);
		};

		glfwSetWindowCloseCallback(m_WindowHandle, callback);
	}

}
