#include "wrapper_glfw_window.h"

#include "glfw_callbacks.h"

#include <GLFW/glfw3.h>

cse::GlfwWindow::GlfwWindow(const int width, const int height, const char* const title) :
	window_ptr(glfwCreateWindow(width, height, title, nullptr, nullptr))
{

}

cse::GlfwWindow::~GlfwWindow()
{
	if (window_ptr != nullptr) {
		unregister_window_for_callbacks(window_ptr);
		glfwDestroyWindow(window_ptr);
	}
}

bool cse::GlfwWindow::is_valid() const
{
	return window_ptr != nullptr;
}