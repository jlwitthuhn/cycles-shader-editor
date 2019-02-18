#pragma once

// This header defines small inline-able functions to make standard GLFW functions work with
// this project's GlfwWindow wrapper class as well as smart pointers to GlfwWindow

#include <memory>

#include <GLFW/glfw3.h>

#include "wrapper_glfw_window.h"

namespace CyclesShaderEditor {

	// Context setup
	inline void glfwMakeContextCurrent(const std::unique_ptr<GlfwWindow>& glfw_window) {
		glfwMakeContextCurrent(glfw_window->window_ptr);
	}

	// Input callback setup
	inline GLFWkeyfun glfwSetKeyCallback(const std::unique_ptr<GlfwWindow>& glfw_window, const GLFWkeyfun cbfun) {
		return glfwSetKeyCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWmousebuttonfun glfwSetMouseButtonCallback(const std::unique_ptr<GlfwWindow>& glfw_window, const GLFWmousebuttonfun cbfun) {
		return glfwSetMouseButtonCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWcharfun glfwSetCharCallback(const std::unique_ptr<GlfwWindow>& glfw_window, const GLFWcharfun cbfun) {
		return glfwSetCharCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWscrollfun glfwSetScrollCallback(const std::unique_ptr<GlfwWindow>& glfw_window, const GLFWscrollfun cbfun) {
		return glfwSetScrollCallback(glfw_window->window_ptr, cbfun);
	}

	// Window status
	inline int glfwWindowShouldClose(const std::unique_ptr<GlfwWindow>& glfw_window) {
		return glfwWindowShouldClose(glfw_window->window_ptr);
	}
	inline void glfwGetFramebufferSize(const std::unique_ptr<GlfwWindow>& glfw_window, int* const width, int* const height) {
		glfwGetFramebufferSize(glfw_window->window_ptr, width, height);
	}
	inline void glfwGetCursorPos(const std::unique_ptr<GlfwWindow>& glfw_window, double* const xpos, double* const ypos) {
		glfwGetCursorPos(glfw_window->window_ptr, xpos, ypos);
	}
	inline void glfwGetWindowSize(const std::unique_ptr<GlfwWindow>& glfw_window, int* const width, int* const height) {
		glfwGetWindowSize(glfw_window->window_ptr, width, height);
	}

	// OpenGL stuff
	inline void glfwSwapBuffers(const std::unique_ptr<GlfwWindow>& glfw_window) {
		glfwSwapBuffers(glfw_window->window_ptr);
	}
}
