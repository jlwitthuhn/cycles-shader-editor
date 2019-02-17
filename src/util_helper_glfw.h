#pragma once

// This header defines small inline-able functions to make standard GLFW functions work with
// this project's GlfwWindow wrapper class as well as smart pointers to GlfwWindow

#include <memory>

#include <GLFW/glfw3.h>

#include "glfw_window.h"

namespace CyclesShaderEditor {

	// Context setup
	inline void glfwMakeContextCurrent(std::unique_ptr<GlfwWindow>& glfw_window) {
		glfwMakeContextCurrent(glfw_window->window_ptr);
	}

	// Input callback setup
	inline GLFWkeyfun glfwSetKeyCallback(std::unique_ptr<GlfwWindow>& glfw_window, const GLFWkeyfun cbfun) {
		return glfwSetKeyCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWmousebuttonfun glfwSetMouseButtonCallback(std::unique_ptr<GlfwWindow>& glfw_window, const GLFWmousebuttonfun cbfun) {
		return glfwSetMouseButtonCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWcharfun glfwSetCharCallback(std::unique_ptr<GlfwWindow>& glfw_window, const GLFWcharfun cbfun) {
		return glfwSetCharCallback(glfw_window->window_ptr, cbfun);
	}
	inline GLFWscrollfun glfwSetScrollCallback(std::unique_ptr<GlfwWindow>& glfw_window, const GLFWscrollfun cbfun) {
		return glfwSetScrollCallback(glfw_window->window_ptr, cbfun);
	}

	// Window status
	inline int glfwWindowShouldClose(std::unique_ptr<GlfwWindow>& glfw_window) {
		return glfwWindowShouldClose(glfw_window->window_ptr);
	}
	inline void glfwGetFramebufferSize(std::unique_ptr<GlfwWindow>& glfw_window, int* const width, int* const height) {
		glfwGetFramebufferSize(glfw_window->window_ptr, width, height);
	}
	inline void glfwGetCursorPos(std::unique_ptr<GlfwWindow>& glfw_window, double* const xpos, double* const ypos) {
		glfwGetCursorPos(glfw_window->window_ptr, xpos, ypos);
	}
	inline void glfwGetWindowSize(std::unique_ptr<GlfwWindow>& glfw_window, int* const width, int* const height) {
		glfwGetWindowSize(glfw_window->window_ptr, width, height);
	}

	// OpenGL stuff
	inline void glfwSwapBuffers(std::unique_ptr<GlfwWindow>& glfw_window) {
		glfwSwapBuffers(glfw_window->window_ptr);
	}
}
