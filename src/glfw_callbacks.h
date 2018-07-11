#pragma once

#include <map>

struct GLFWwindow;

namespace CyclesShaderEditor {

	class EditorMainWindow;

	std::map<GLFWwindow*, EditorMainWindow*>* get_callback_window_map();

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void character_callback(GLFWwindow* window, unsigned int codepoint);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

}
