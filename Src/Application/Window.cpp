

// SPDX - License - Identifier: MIT
// Copyright(c) 2025 Jayantkumar56


#include "Window.h"

#include <iostream>


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

std::optional<Window> Window::Create(const std::string_view title, 
	                                 const int width, 
	                                 const int height) noexcept {
	if (!s_GLFWInitialised) {
		std::cerr << "GLFW not initialised." << std::endl;
		return std::nullopt;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.data(), NULL, NULL);

	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return std::nullopt;
	}

	glfwMakeContextCurrent(window);

	if (!s_GladInitialized && !InitialiseGlad()) {
		return std::nullopt;
	}

	return Window(window);
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
