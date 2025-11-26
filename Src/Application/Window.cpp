

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Window.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"
#include "Events/EventDispatcher.h"
#include "Utils/Logger.h"


namespace Mct {

	bool Window::s_GLFWInitialised = false;
	bool Window::s_GladInitialized = false;

	static void APIENTRY GLDebugCallback(GLenum        source, 
									     GLenum        type, 
									     GLuint        id,
										 GLenum        severity, 
									     GLsizei       length,
										 const GLchar* message, 
									     const void*   userParam) noexcept;

	bool Window::Initialise() noexcept {
		if (!glfwInit()) {
			MCT_ERROR("Failed to initialize GLFW");
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
			MCT_ERROR("GLFW not initialised before creation of window " 
				      "(Call Window::Initialise before calling Window::Create).");
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		GLFWwindow* windowHandle = glfwCreateWindow(width, height, title.data(), NULL, NULL);

		if (windowHandle == NULL) {
			MCT_ERROR("Failed to create GLFW window.");
			glfwTerminate();
			return nullptr;
		}

		glfwMakeContextCurrent(windowHandle);

		if (!s_GladInitialized) {
			if (!InitialiseGlad())
				return nullptr;
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
			MCT_ERROR("Failed to initialize GLAD");
			s_GladInitialized = false;
			return false;
		}

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(GLDebugCallback, 0);

		s_GladInitialized = true;
		return true;
	}

	void Window::SetKeyCallback() {
		static GLFWkeyfun callback = +[](GLFWwindow* window, int key, int scancode, int action, int mods) {
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
		static GLFWcharfun callback = +[](GLFWwindow* window, unsigned int codepoint) {
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
		static GLFWmousebuttonfun callback = +[](GLFWwindow* window, int button, int action, int mods) {
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
		static GLFWscrollfun callback = +[](GLFWwindow* window, double xoffset, double yoffset) {
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
		static GLFWcursorposfun callback = +[](GLFWwindow* window, double xpos, double ypos) {
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
		static GLFWwindowsizefun callback = +[](GLFWwindow* window, int width, int height) {
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
		static GLFWwindowclosefun callback = +[](GLFWwindow* window) {
			auto* wind = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (wind->m_EventCallbacks && wind->m_EventCallbacks->WindowClose)
				wind->m_EventCallbacks->WindowClose(window);

			WindowCloseEvent e{};
			EventDispatcher::DispatchEvent(e);
		};

		glfwSetWindowCloseCallback(m_WindowHandle, callback);
	}

	static void APIENTRY GLDebugCallback(GLenum        source, 
									     GLenum        type, 
									     GLuint        id,
										 GLenum        severity, 
									     GLsizei       length,
										 const GLchar* message, 
									     const void*   userParam) noexcept
	{
		if (type == GL_DEBUG_TYPE_ERROR || severity == GL_DEBUG_SEVERITY_HIGH) {
			MCT_ERROR("[GL ERROR] id={} type={} severity={} msg={}", id, type, severity, message);
			MCT_DEBUGBREAK();
		}
		else {
			MCT_WARN("[GL] id={} type={} severity={} msg={}", id, type, severity, message);
		}
	}

}
