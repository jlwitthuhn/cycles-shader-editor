#include "glfw_callbacks.h"

#include <GLFW/glfw3.h>

#include "main_window.h"

std::map<GLFWwindow*, CyclesShaderEditor::EditorMainWindow*>* CyclesShaderEditor::get_callback_window_map()
{
	static std::map<GLFWwindow*, EditorMainWindow*> window_map;
	return &window_map;
}

void CyclesShaderEditor::mouse_button_callback(GLFWwindow* const window, const int button, const int action, const int mods)
{
	std::map<GLFWwindow*, EditorMainWindow*>* const window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* const node_editor = window_map_ptr->operator[](window);
		node_editor->handle_mouse_button(button, action, mods);
	}
}

void CyclesShaderEditor::key_callback(GLFWwindow* const window, const int key, const int scancode, const int action, const int mods)
{
	std::map<GLFWwindow*, EditorMainWindow*>* const window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* const node_editor = window_map_ptr->operator[](window);
		node_editor->handle_key(key, scancode, action, mods);
	}
}

void CyclesShaderEditor::character_callback(GLFWwindow* const window, const unsigned int codepoint)
{
	std::map<GLFWwindow*, EditorMainWindow*>* const window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* const node_editor = window_map_ptr->operator[](window);
		node_editor->handle_character(codepoint);
	}
}

void CyclesShaderEditor::scroll_callback(GLFWwindow* const window, const double xoffset, const double yoffset)
{
	std::map<GLFWwindow*, EditorMainWindow*>* const window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* const node_editor = window_map_ptr->operator[](window);
		node_editor->handle_scroll(xoffset, yoffset);
	}
}