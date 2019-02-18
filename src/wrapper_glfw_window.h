#pragma once

struct GLFWwindow;

namespace CyclesShaderEditor {

	// Class to safely wrap glfwCreateWindow and glfwDestroyWindow
	class GlfwWindow {
	public:
		GlfwWindow(int width, int height, const char* title);
		~GlfwWindow();

		bool is_valid() const;

		GLFWwindow* const window_ptr;
	};
}
