#include "main_window.h"

#define NANOVG_GL2_IMPLEMENTATION

#include <utility>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nanovg.h>
#include <nanovg_gl.h>

#include "common_enums.h"
#include "glfw_callbacks.h"
#include "glfw_window.h"
#include "gui_sizes.h"
#include "graph_editor.h"
#include "node_base.h"
#include "output.h"
#include "serialize.h"
#include "sockets.h"
#include "statusbar.h"
#include "subwindow.h"
#include "subwindow_node_list.h"
#include "subwindow_param_editor.h"
#include "toolbar.h"
#include "util_platform.h"
#include "view.h"

CyclesShaderEditor::EditorMainWindow::EditorMainWindow(GraphEditor* public_window) :
	main_graph(ShaderGraphType::MATERIAL),
	public_window(public_window)
{
	window_width = UI_WINDOW_WIDTH;
	window_height = UI_WINDOW_HEIGHT;

	nvg_context = nullptr;

	last_buffer_swap_time = std::chrono::steady_clock::now();

	node_creation_helper = std::make_shared<NodeCreationHelper>();
}

CyclesShaderEditor::EditorMainWindow::~EditorMainWindow()
{
	release_resources();
	glfwTerminate();
}

void CyclesShaderEditor::EditorMainWindow::set_font_search_path(const PathString& path)
{
	font_search_path = path;
}

bool CyclesShaderEditor::EditorMainWindow::create_window()
{
	if (!glfwInit()) {
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfw_window = std::make_unique<GlfwWindow>(window_width, window_height, "Node Graph Editor");
	if (glfw_window->is_valid() == false) {
		return false;
	}

	glfwMakeContextCurrent(glfw_window->window_ptr);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK) {
		return false;
	}

	nvg_context = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (nvg_context == nullptr) {
		return false;
	}

	glfwSwapInterval(0);

	glfwSetTime(0);

	glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

	const PathString sans_font_path = Platform::get_font_path(font_search_path, "SourceSansPro-Regular.ttf");
	Platform::nvg_create_font(sans_font_path, "sans", nvg_context);

	register_window_pair_for_callbacks(glfw_window->window_ptr, this);

	glfwSetKeyCallback(glfw_window->window_ptr, key_callback);
	glfwSetMouseButtonCallback(glfw_window->window_ptr, mouse_button_callback);
	glfwSetCharCallback(glfw_window->window_ptr, character_callback);
	glfwSetScrollCallback(glfw_window->window_ptr, scroll_callback);

	toolbar = std::make_unique<NodeEditorToolbar>();
	status_bar = std::make_unique<NodeEditorStatusBar>();

	std::unique_ptr<NodeEditorSubwindow> node_list_window =
		std::make_unique<NodeListSubwindow>(node_creation_helper, CyclesShaderEditor::FloatPos(15.0f, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	std::unique_ptr<NodeEditorSubwindow> param_editor_window =
		std::make_unique<ParamEditorSubwindow>(CyclesShaderEditor::FloatPos(15.0f * 2.0f + UI_SUBWIN_NODE_LIST_WIDTH, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	subwindows.push_back(std::move(node_list_window));
	subwindows.push_back(std::move(param_editor_window));

	view = std::make_unique<EditGraphView>(&main_graph, node_creation_helper);

	update_serialized_state();

	return true;
}

bool CyclesShaderEditor::EditorMainWindow::run_window_loop_iteration()
{
	if (glfwWindowShouldClose(glfw_window->window_ptr)) {
		return false;
	}

	pre_draw();

	int fb_width, fb_height;
	glfwGetFramebufferSize(glfw_window->window_ptr, &fb_width, &fb_height);
	const float px_ratio = static_cast<float>(fb_width) / window_width;

	glViewport(0, 0, fb_width, fb_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	const float window_width_fl = static_cast<float>(window_width);
	const float window_height_fl = static_cast<float>(window_height);
	nvgBeginFrame(nvg_context, window_width_fl, window_height_fl, px_ratio);
	draw();
	nvgEndFrame(nvg_context);

	swap_buffers();

	return true;
}

void CyclesShaderEditor::EditorMainWindow::set_target_frame_rate(const double fps)
{
	target_frame_rate = fps;
}

void CyclesShaderEditor::EditorMainWindow::handle_mouse_button(const int button, const int action, const int mods)
{
	// Clicking anywhere will deselect any input box
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (const auto& this_subwindow : subwindows) {
			this_subwindow->deselect_input_box();
		}
	}

	const bool toolbar_has_focus = (toolbar != nullptr && toolbar->is_mouse_over());

	if (forward_mouse_to_subwindow(button, action, mods)) {
		// Input has already been handled, do nothing
	}
	else if (toolbar_has_focus) {
		toolbar->handle_mouse_button(button, action, mods);
	}
	else {
		// View has focus
		view->handle_mouse_button(button, action, mods);
	}

	// Special case for mouse button release, notify all ui elements so click+drag works
	if (action == GLFW_RELEASE) {
		view->handle_mouse_button(button, action, mods);
		toolbar->handle_mouse_button(button, action, mods);
		for (const auto& this_subwindow : subwindows) {
			this_subwindow->handle_mouse_button(button, action, mods);
		}
	}
}

void CyclesShaderEditor::EditorMainWindow::handle_key(const int key, const int scancode, const int action, const int mods)
{
	// Global shortcuts that should be handled with greater priority than anything else
	if (mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_S:
				requests.save = true;
				return;
			case GLFW_KEY_Z:
				requests.undo = true;
				return;
			case GLFW_KEY_Y:
				requests.redo = true;
				return;
			default:
				break;
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_LEFT:
			requests.view.pan_left = true;
			return;
		case GLFW_KEY_RIGHT:
			requests.view.pan_right = true;
			return;
		case GLFW_KEY_UP:
			requests.view.pan_up = true;
			return;
		case GLFW_KEY_DOWN:
			requests.view.pan_down = true;
			return;
		default:
			break;
		}
	}

	if (forward_key_to_subwindow(key, scancode, action, mods)) {
		// Subwindow took the input, end here
		return;
	}

	// If no subwindow has grabbed the input, direct keys to the view
	view->handle_key(key, scancode, action, mods);
}

void CyclesShaderEditor::EditorMainWindow::handle_character(const unsigned int codepoint)
{
	forward_character_to_subwindow(codepoint);
}

void CyclesShaderEditor::EditorMainWindow::handle_scroll(const double /*xoffset*/, const double yoffset)
{
	if (yoffset > 0.1) {
		requests.view.zoom_in = true;
	}
	else if (yoffset < -0.1) {
		requests.view.zoom_out = true;
	}
}

void CyclesShaderEditor::EditorMainWindow::load_serialized_graph(const std::string& graph_str)
{
	clear_graph(true);
	deserialize_graph(graph_str, this->main_graph.nodes, this->main_graph.connections);
	update_serialized_state();
}

void CyclesShaderEditor::EditorMainWindow::pre_draw()
{
	// Check nodes to see if we should save current state
	{
		bool should_push_undo_state = false;
		for (auto& this_subwindow : subwindows) {
			if (this_subwindow->needs_undo_push()) {
				should_push_undo_state = true;
			}
		}
		if (main_graph.needs_undo_push()) {
			should_push_undo_state = true;
		}
		if (view->needs_undo_push()) {
			should_push_undo_state = true;
		}
		if (should_push_undo_state) {
			push_undo_state();
		}
	}

	// Get new mouse position and window size
	double mx, my;
	glfwGetCursorPos(glfw_window->window_ptr, &mx, &my);
	glfwGetWindowSize(glfw_window->window_ptr, &window_width, &window_height);
	update_mouse_position(CyclesShaderEditor::FloatPos(static_cast<float>(mx), static_cast<float>(my)));

	// Handle window events
	glfwPollEvents();

	// Grab requests from all ui components before acting on them
	requests |= toolbar->consume_ui_requests();

	// Handle internal requests
	service_requests();

	// Update mouse position for all subthings
	toolbar->set_mouse_position(mouse_screen_pos);
	view->set_mouse_position(mouse_screen_pos, window_width, window_height);
	for (auto& subwindow : subwindows) {
		const float max_safe_pos_y = window_height - UI_STATUSBAR_HEIGHT;
		const FloatPos subwindow_pos = subwindow->get_screen_pos();
		const FloatPos local_mouse_pos = mouse_screen_pos - subwindow_pos;
		subwindow->set_mouse_position(local_mouse_pos, max_safe_pos_y);
	}

	// Update any other state we need
	view->update();
	status_bar->set_zoom_text(view->get_zoom_string());
	for (auto& subwindow : subwindows) {
		subwindow->update_selection(view->get_const_selection());
	}

	// Update toolbar button state
	if (undo_stack.undo_available()) {
		toolbar->set_button_enabled(CyclesShaderEditor::ToolbarButtonType::UNDO, true);
	}
	else {
		toolbar->set_button_enabled(CyclesShaderEditor::ToolbarButtonType::UNDO, false);
	}
	if (undo_stack.redo_available()) {
		toolbar->set_button_enabled(CyclesShaderEditor::ToolbarButtonType::REDO, true);
	}
	else {
		toolbar->set_button_enabled(CyclesShaderEditor::ToolbarButtonType::REDO, false);
	}

	// Mark all connected input sockets
	for (NodeConnection& connection : main_graph.connections) {
		if (auto connection_end = connection.end_socket.lock()) {
			connection_end->input_connected_this_frame = true;
		}
	}

	for (auto& this_subwindow : subwindows) {
		this_subwindow->pre_draw();
	}
}

void CyclesShaderEditor::EditorMainWindow::draw()
{
	// Draw view
	nvgSave(nvg_context);
	view->draw(nvg_context);
	nvgRestore(nvg_context);

	// Draw toolbar
	if (toolbar != nullptr) {
		toolbar->draw(nvg_context, static_cast<float>(window_width));
	}

	// Draw subwindows
	std::list<std::unique_ptr<NodeEditorSubwindow>>::reverse_iterator window_iter;
	for (window_iter = subwindows.rbegin(); window_iter != subwindows.rend(); ++window_iter) {
		const FloatPos subwindow_pos = (*window_iter)->get_screen_pos();
		nvgSave(nvg_context);
		nvgTranslate(nvg_context, subwindow_pos.get_x(), subwindow_pos.get_y());
		(*window_iter)->draw(nvg_context);
		nvgRestore(nvg_context);
	}

	// Draw status bar
	if (status_bar != nullptr) {
		const float status_bar_height = NodeEditorStatusBar::get_status_bar_height();
		nvgSave(nvg_context);
		nvgTranslate(nvg_context, 0.0f, static_cast<float>(window_height) - status_bar_height);
		status_bar->draw(nvg_context, static_cast<float>(window_width));
		nvgRestore(nvg_context);
	}
}

void CyclesShaderEditor::EditorMainWindow::swap_buffers()
{
	if (target_frame_rate > 0.0) {
		const auto current_time = std::chrono::steady_clock::now();
		if (current_time.time_since_epoch().count() > last_buffer_swap_time.time_since_epoch().count()) {
			const std::chrono::duration<double> this_frame_duration = static_cast<std::chrono::duration<double> >(current_time - last_buffer_swap_time);
			const double desired_frame_duration = 1.0 / target_frame_rate;
			const double sleep_time_sec = (desired_frame_duration - this_frame_duration.count());
			const int sleep_time_us = static_cast<int>(sleep_time_sec * 1000 * 1000);
			const int max_sleep_time_us = static_cast<int>(desired_frame_duration * 1000 * 1000);
			if (sleep_time_us > 0 && sleep_time_us < max_sleep_time_us) {
				Platform::thread_usleep(sleep_time_us);
			}
		}
	}
	last_buffer_swap_time = std::chrono::steady_clock::now();
	glfwSwapBuffers(glfw_window->window_ptr);
}

void CyclesShaderEditor::EditorMainWindow::service_requests()
{
	if (requests.save) {
		do_output();
		requests.save = false;
	}
	if (requests.undo) {
		undo();
		requests.undo = false;
	}
	if (requests.redo) {
		redo();
		requests.redo = false;
	}
	view->handle_requests(requests.view);
}

void CyclesShaderEditor::EditorMainWindow::update_mouse_position(CyclesShaderEditor::FloatPos screen_position)
{
	screen_position.clamp_to(CyclesShaderEditor::FloatPos(0.0f, 0.0f), CyclesShaderEditor::FloatPos(window_width - 1.0f, window_height - 1.0f));
	mouse_screen_pos = screen_position;
}

bool CyclesShaderEditor::EditorMainWindow::forward_mouse_to_subwindow(const int button, const int action, const int mods)
{
	std::list<std::unique_ptr<NodeEditorSubwindow>>::iterator iter;
	for (iter = subwindows.begin(); iter != subwindows.end(); ++iter) {
		auto& subwindow_ptr = *iter;
		if (subwindow_ptr->is_mouse_over()) {
			subwindow_ptr->handle_mouse_button(button, action, mods);
			std::unique_ptr<NodeEditorSubwindow> new_ptr = std::move(*iter);
			subwindows.erase(iter);
			subwindows.push_front(std::move(new_ptr));
			return true;
		}
	}
	return false;
}

bool CyclesShaderEditor::EditorMainWindow::forward_key_to_subwindow(const int key, const int scancode, const int action, const int mods)
{
	for (auto& this_subwindow : subwindows) {
		if (this_subwindow->should_capture_input()) {
			this_subwindow->handle_key(key, scancode, action, mods);
			return true;
		}
	}
	return false;
}

bool CyclesShaderEditor::EditorMainWindow::forward_character_to_subwindow(const unsigned int codepoint)
{
	for (auto& this_subwindow : subwindows) {
		if (this_subwindow->should_capture_input()) {
			this_subwindow->handle_character(codepoint);
			return true;
		}
	}
	return false;
}

void CyclesShaderEditor::EditorMainWindow::update_serialized_state()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;
	generate_output_lists(main_graph.nodes, main_graph.connections, out_nodes, out_connections);
	serialized_state = serialize_graph(out_nodes, out_connections);
}

void CyclesShaderEditor::EditorMainWindow::push_undo_state()
{
	undo_stack.push_undo_state(serialized_state);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void CyclesShaderEditor::EditorMainWindow::undo()
{
	if (undo_stack.undo_available() == false) {
		return;
	}
	update_serialized_state();
	const std::string new_state = undo_stack.pop_undo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, main_graph.nodes, main_graph.connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void CyclesShaderEditor::EditorMainWindow::redo()
{
	if (undo_stack.redo_available() == false) {
		return;
	}
	update_serialized_state();
	const std::string new_state = undo_stack.pop_redo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, main_graph.nodes, main_graph.connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void CyclesShaderEditor::EditorMainWindow::clear_graph(const bool reset_undo)
{
	if (reset_undo) {
		undo_stack.clear();
	}
	view->clear_selection();
	main_graph.nodes.clear();
	main_graph.connections.clear();
	update_serialized_state();
}

void CyclesShaderEditor::EditorMainWindow::do_output()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;

	generate_output_lists(main_graph.nodes, main_graph.connections, out_nodes, out_connections);

	clear_graph(false);

	public_window->serialized_output = serialize_graph(out_nodes, out_connections);

	// Re-create graph from saved state so serialization errors are more apparent
	deserialize_graph(public_window->serialized_output, main_graph.nodes, main_graph.connections);
	update_serialized_state();

	public_window->output_updated = true;
	status_bar->set_status_text("Saved");
}

void CyclesShaderEditor::EditorMainWindow::release_resources()
{
	subwindows.clear();

	main_graph.connections.clear();
	for (EditorNode* const this_node : main_graph.nodes) {
		delete this_node;
	}
	main_graph.nodes.clear();

	toolbar.reset();
	status_bar.reset();
	view.reset();

	glfw_window.release();

	if (nvg_context != nullptr) {
		nvgDeleteGL2(nvg_context);
		nvg_context = nullptr;
	}
}
