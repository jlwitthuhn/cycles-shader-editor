#include "glfw_callbacks.h"

#include <GLFW/glfw3.h>

#include "main_window.h"

#include <iostream>

std::map<GLFWwindow*, CyclesShaderEditor::EditorMainWindow*>* CyclesShaderEditor::get_callback_window_map()
{
	static std::map<GLFWwindow*, EditorMainWindow*> window_map;
	return &window_map;
}

void CyclesShaderEditor::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::map<GLFWwindow*, EditorMainWindow*>* window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* node_editor = window_map_ptr->operator[](window);
		node_editor->handle_mouse_button(button, action, mods);
	}
}

void CyclesShaderEditor::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::map<GLFWwindow*, EditorMainWindow*>* window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* node_editor = window_map_ptr->operator[](window);
		node_editor->handle_key(key, scancode, action, mods);
	}
}

void CyclesShaderEditor::character_callback(GLFWwindow* window, unsigned int codepoint)
{
	std::map<GLFWwindow*, EditorMainWindow*>* window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* node_editor = window_map_ptr->operator[](window);
		node_editor->handle_character(codepoint);
	}
}

void CyclesShaderEditor::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::map<GLFWwindow*, EditorMainWindow*>* window_map_ptr = get_callback_window_map();
	if (window_map_ptr->count(window) == 1) {
		EditorMainWindow* node_editor = window_map_ptr->operator[](window);
		node_editor->handle_scroll(xoffset, yoffset);
	}
}