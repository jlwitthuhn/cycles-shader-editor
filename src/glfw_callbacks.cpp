#include "glfw_callbacks.h"

#include <map>
#include <mutex>

#include <GLFW/glfw3.h>

#include "main_window.h"

std::mutex window_map_mutex;
std::map<GLFWwindow*, CyclesShaderEditor::EditorMainWindow*> window_map;

static CyclesShaderEditor::EditorMainWindow* get_main_window(GLFWwindow* const glfw_window)
{
	std::lock_guard<std::mutex> lock(window_map_mutex);
	if (window_map.count(glfw_window) > 0) {
		return window_map[glfw_window];
	}
	else {
		return nullptr;
	}
}

void CyclesShaderEditor::register_window_pair_for_callbacks(GLFWwindow* const glfw_window, EditorMainWindow* const main_window)
{
	std::lock_guard<std::mutex> lock(window_map_mutex);
	window_map[glfw_window] = main_window;
}

void CyclesShaderEditor::unregister_window_for_callbacks(GLFWwindow* const glfw_window)
{
	std::lock_guard<std::mutex> lock(window_map_mutex);
	window_map.erase(glfw_window);
}

void CyclesShaderEditor::mouse_button_callback(GLFWwindow* const window, const int button, const int action, const int mods)
{
	EditorMainWindow* const main_window = get_main_window(window);
	if (main_window != nullptr) {
		main_window->handle_mouse_button(button, action, mods);
	}
}

void CyclesShaderEditor::key_callback(GLFWwindow* const window, const int key, const int scancode, const int action, const int mods)
{
	EditorMainWindow* const main_window = get_main_window(window);
	if (main_window != nullptr) {
		main_window->handle_key(key, scancode, action, mods);
	}
}

void CyclesShaderEditor::character_callback(GLFWwindow* const window, const unsigned int codepoint)
{
	EditorMainWindow* const main_window = get_main_window(window);
	if (main_window != nullptr) {
		main_window->handle_character(codepoint);
	}
}

void CyclesShaderEditor::scroll_callback(GLFWwindow* const window, const double xoffset, const double yoffset)
{
	EditorMainWindow* const main_window = get_main_window(window);
	if (main_window != nullptr) {
		main_window->handle_scroll(xoffset, yoffset);
	}
}
