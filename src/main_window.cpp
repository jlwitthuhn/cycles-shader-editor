#include "main_window.h"

#include <utility>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "editable_graph.h"
#include "glfw_callbacks.h"
#include "gui_sizes.h"
#include "node_base.h"
#include "output.h"
#include "serialize.h"
#include "sockets.h"
#include "statusbar.h"
#include "subwindow.h"
#include "subwindow_node_list.h"
#include "subwindow_param_editor.h"
#include "toolbar.h"
#include "util_enum.h"
#include "util_platform.h"
#include "view.h"
#include "wrapper_glfw_func.h"
#include "wrapper_glfw_window.h"
#include "wrapper_nvg_context.h"
#include "wrapper_nvg_func.h"

cse::EditorMainWindow::EditorMainWindow() :
	main_graph(std::make_shared<EditableGraph>(ShaderGraphType::MATERIAL))
{
	window_width = UI_WINDOW_WIDTH;
	window_height = UI_WINDOW_HEIGHT;

	last_buffer_swap_time = std::chrono::steady_clock::now();

	node_creation_helper = std::make_shared<NodeCreationHelper>();
}

cse::EditorMainWindow::~EditorMainWindow()
{
	release_resources();
	glfwTerminate();
}

void cse::EditorMainWindow::set_font_search_path(const PathString& path)
{
	font_search_path = path;
}

bool cse::EditorMainWindow::create_window()
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

	glfwMakeContextCurrent(glfw_window);
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK) {
		return false;
	}

	nvg_context = std::make_unique<NvgContext>();
	if (nvg_context->is_valid() == false) {
		return false;
	}

	glfwSwapInterval(0);

	glfwSetTime(0);

	glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

	const PathString sans_font_path = Platform::get_font_path(font_search_path, "SourceSansPro-Regular.ttf");
	Platform::nvg_create_font(sans_font_path, "sans", nvg_context);

	register_window_pair_for_callbacks(glfw_window->window_ptr, this);

	glfwSetKeyCallback(glfw_window, key_callback);
	glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
	glfwSetCharCallback(glfw_window, character_callback);
	glfwSetScrollCallback(glfw_window, scroll_callback);

	toolbar = std::make_unique<NodeEditorToolbar>();
	status_bar = std::make_unique<NodeEditorStatusBar>();

	std::unique_ptr<NodeEditorSubwindow> node_list_window =
		std::make_unique<NodeListSubwindow>(node_creation_helper, cse::Float2(15.0f, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	std::unique_ptr<NodeEditorSubwindow> param_editor_window =
		std::make_unique<ParamEditorSubwindow>(cse::Float2(15.0f * 2.0f + UI_SUBWIN_NODE_LIST_WIDTH, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	subwindows.push_back(std::move(node_list_window));
	subwindows.push_back(std::move(param_editor_window));

	view = std::make_unique<EditGraphView>(main_graph, node_creation_helper);

	update_serialized_state();

	return true;
}

bool cse::EditorMainWindow::run_window_loop_iteration()
{
	if (glfwWindowShouldClose(glfw_window)) {
		return false;
	}

	pre_draw();

	int fb_width, fb_height;
	glfwGetFramebufferSize(glfw_window, &fb_width, &fb_height);
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

void cse::EditorMainWindow::set_target_frame_rate(const double fps)
{
	target_frame_rate = fps;
}

void cse::EditorMainWindow::handle_mouse_button(const int button, const int action, const int mods)
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

void cse::EditorMainWindow::handle_key(const int key, const int scancode, const int action, const int mods)
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

void cse::EditorMainWindow::handle_character(const unsigned int codepoint)
{
	forward_character_to_subwindow(codepoint);
}

void cse::EditorMainWindow::handle_scroll(const double /*xoffset*/, const double yoffset)
{
	if (yoffset > 0.1) {
		requests.view.zoom_in = true;
	}
	else if (yoffset < -0.1) {
		requests.view.zoom_out = true;
	}
}

void cse::EditorMainWindow::load_serialized_graph(const std::string& graph_str)
{
	clear_graph(true);
	deserialize_graph(graph_str, this->main_graph->nodes, this->main_graph->connections);
	update_serialized_state();
}

bool cse::EditorMainWindow::get_serialized_output(std::string& graph) {
	graph = serialized_output;
	if (serialized_output_updated) {
		serialized_output_updated = false;
		return true;
	}
	else {
		return false;
	}
}

void cse::EditorMainWindow::pre_draw()
{
	// Check nodes to see if we should save current state
	{
		bool should_push_undo_state = false;
		for (auto& this_subwindow : subwindows) {
			if (this_subwindow->needs_undo_push()) {
				should_push_undo_state = true;
			}
		}
		if (main_graph->needs_undo_push()) {
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
	glfwGetCursorPos(glfw_window, &mx, &my);
	glfwGetWindowSize(glfw_window, &window_width, &window_height);
	update_mouse_position(cse::Float2(static_cast<float>(mx), static_cast<float>(my)));

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
		const Float2 subwindow_pos = subwindow->get_screen_pos();
		const Float2 local_mouse_pos = mouse_screen_pos - subwindow_pos;
		subwindow->set_mouse_position(local_mouse_pos, max_safe_pos_y);
	}

	// Update any other state we need
	view->pre_draw();
	status_bar->set_zoom_text(view->get_zoom_string());
	for (auto& subwindow : subwindows) {
		subwindow->update_selection(view->get_const_selection());
	}

	// Mark all connected input sockets
	for (NodeConnection& connection : main_graph->connections) {
		if (auto connection_end = connection.end_socket.lock()) {
			connection_end->input_connected_this_frame = true;
		}
	}

	// Update toolbar button state
	toolbar->set_button_enabled(cse::ToolbarButtonType::UNDO, undo_stack.undo_available());
	toolbar->set_button_enabled(cse::ToolbarButtonType::REDO, undo_stack.redo_available());

	// Run all UI pre_draw last
	for (auto& this_subwindow : subwindows) {
		this_subwindow->pre_draw();
	}
}

void cse::EditorMainWindow::draw()
{
	NVGcontext* const nvg_ctx_pointer = nvg_context->context_ptr;

	// Draw view
	nvgSave(nvg_ctx_pointer);
	view->draw(nvg_ctx_pointer);
	nvgRestore(nvg_ctx_pointer);

	// Draw toolbar
	if (toolbar != nullptr) {
		toolbar->draw(nvg_ctx_pointer, static_cast<float>(window_width));
	}

	// Draw subwindows
	std::list<std::unique_ptr<NodeEditorSubwindow>>::reverse_iterator window_iter;
	for (window_iter = subwindows.rbegin(); window_iter != subwindows.rend(); ++window_iter) {
		const Float2 subwindow_pos = (*window_iter)->get_screen_pos();
		nvgSave(nvg_ctx_pointer);
		nvgTranslate(nvg_ctx_pointer, subwindow_pos.x, subwindow_pos.y);
		(*window_iter)->draw(nvg_ctx_pointer);
		nvgRestore(nvg_ctx_pointer);
	}

	// Draw status bar
	if (status_bar != nullptr) {
		const float status_bar_height = NodeEditorStatusBar::get_status_bar_height();
		nvgSave(nvg_ctx_pointer);
		nvgTranslate(nvg_ctx_pointer, 0.0f, static_cast<float>(window_height) - status_bar_height);
		status_bar->draw(nvg_ctx_pointer, static_cast<float>(window_width));
		nvgRestore(nvg_ctx_pointer);
	}
}

void cse::EditorMainWindow::swap_buffers()
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
	glfwSwapBuffers(glfw_window);
}

void cse::EditorMainWindow::service_requests()
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

void cse::EditorMainWindow::update_mouse_position(const cse::Float2 screen_position)
{
	mouse_screen_pos = screen_position.clamp_to(cse::Float2(0.0f, 0.0f), cse::Float2(window_width - 1.0f, window_height - 1.0f));
}

bool cse::EditorMainWindow::forward_mouse_to_subwindow(const int button, const int action, const int mods)
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

bool cse::EditorMainWindow::forward_key_to_subwindow(const int key, const int scancode, const int action, const int mods)
{
	for (auto& this_subwindow : subwindows) {
		if (this_subwindow->has_input_focus()) {
			this_subwindow->handle_key(key, scancode, action, mods);
			return true;
		}
	}
	return false;
}

bool cse::EditorMainWindow::forward_character_to_subwindow(const unsigned int codepoint)
{
	for (auto& this_subwindow : subwindows) {
		if (this_subwindow->has_input_focus()) {
			this_subwindow->handle_character(codepoint);
			return true;
		}
	}
	return false;
}

void cse::EditorMainWindow::update_serialized_state()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;
	generate_output_lists(main_graph->nodes, main_graph->connections, out_nodes, out_connections);
	serialized_state = serialize_graph(out_nodes, out_connections);
}

void cse::EditorMainWindow::push_undo_state()
{
	undo_stack.push_undo_state(serialized_state);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void cse::EditorMainWindow::undo()
{
	if (undo_stack.undo_available() == false) {
		return;
	}
	update_serialized_state();
	const std::string new_state = undo_stack.pop_undo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, main_graph->nodes, main_graph->connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void cse::EditorMainWindow::redo()
{
	if (undo_stack.redo_available() == false) {
		return;
	}
	update_serialized_state();
	const std::string new_state = undo_stack.pop_redo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, main_graph->nodes, main_graph->connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void cse::EditorMainWindow::clear_graph(const bool reset_undo)
{
	if (reset_undo) {
		undo_stack.clear();
	}
	view->clear_selection();
	main_graph->nodes.clear();
	main_graph->connections.clear();
	update_serialized_state();
}

void cse::EditorMainWindow::do_output()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;

	generate_output_lists(main_graph->nodes, main_graph->connections, out_nodes, out_connections);

	clear_graph(false);

	serialized_output = serialize_graph(out_nodes, out_connections);
	serialized_output_updated = true;

	// Re-create graph from saved state so serialization errors are more apparent
	deserialize_graph(serialized_output, main_graph->nodes, main_graph->connections);
	update_serialized_state();

	status_bar->set_status_text("Saved");
}

void cse::EditorMainWindow::release_resources()
{
	subwindows.clear();

	main_graph->connections.clear();
	main_graph->nodes.clear();

	toolbar.reset();
	status_bar.reset();
	view.reset();

	glfw_window.release();
	nvg_context.release();
}
