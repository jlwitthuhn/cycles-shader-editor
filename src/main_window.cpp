#include "main_window.h"

#include <cassert>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <nanovg.h>
#include <nanovg_gl.h>

#include "buttons_nodes.h"
#include "glfw_callbacks.h"
#include "gui_sizes.h"
#include "graph_decoder.h"
#include "graph_editor.h"
#include "node_outputs.h"
#include "node_shaders.h"
#include "output.h"
#include "serialize.h"
#include "subwindow_node_list.h"
#include "subwindow_param_editor.h"
#include "util_platform.h"
#include "util_rectangle.h"
#include "view.h"

CyclesShaderEditor::EditorMainWindow::EditorMainWindow(GraphEditor* public_window) : public_window(public_window)
{
	window_width = UI_WINDOW_WIDTH;
	window_height = UI_WINDOW_HEIGHT;

	window = nullptr;
	nvg_context = nullptr;

	last_buffer_swap_time = std::chrono::steady_clock::now();
}

CyclesShaderEditor::EditorMainWindow::~EditorMainWindow()
{
	release_resources();
}

void CyclesShaderEditor::EditorMainWindow::set_font_search_path(PathString path)
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

	window = glfwCreateWindow(window_width, window_height, "Node Graph Editor", nullptr, nullptr);
	if (window == nullptr) {
		return false;
	}

	glfwMakeContextCurrent(window);
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

#ifdef _WIN32
	std::wstring sans_font_path = font_search_path + L"\\SourceSansPro-Regular.ttf";
	nvgCreateFontW(nvg_context, "sans", sans_font_path.c_str());
#else
	std::string sans_font_path = font_search_path + "/SourceSansPro-Regular.ttf";
	nvgCreateFont(nvg_context, "sans", sans_font_path.c_str());
#endif

	(*get_callback_window_map())[window] = this;
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetScrollCallback(window, scroll_callback);

	toolbar = new NodeEditorToolbar(&requests);
	status_bar = new NodeEditorStatusBar();

	node_list_window = new NodeListSubwindow(CyclesShaderEditor::Point2(15.0f, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	param_editor_window = new ParamEditorSubwindow(CyclesShaderEditor::Point2(15.0f * 2.0f + UI_SUBWIN_NODE_LIST_WIDTH, NodeEditorToolbar::get_toolbar_height() + 15.0f));
	subwindows.push_back(node_list_window);
	subwindows.push_back(param_editor_window);

	EditorNode* output_node = new MaterialOutputNode(CyclesShaderEditor::Point2(0.0f, 0.0f));
	nodes.push_back(output_node);

	view = new EditGraphView(
		nodes,
		connections
	);

	update_serialized_state();

	return true;
}

bool CyclesShaderEditor::EditorMainWindow::run_window_loop_iteration()
{
	if (glfwWindowShouldClose(window)) {
		return false;
	}

	// Pre-draw
	pre_draw();

	// Draw frame
	int fb_width, fb_height;
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	const float px_ratio = static_cast<float>(fb_width) / window_width;

	glViewport(0, 0, fb_width, fb_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(nvg_context, window_width, window_height, px_ratio);
	draw();
	nvgEndFrame(nvg_context);
	swap_buffers();

	return true;
}

void CyclesShaderEditor::EditorMainWindow::set_target_frame_rate(double fps)
{
	target_frame_rate = fps;
}

void CyclesShaderEditor::EditorMainWindow::handle_mouse_button(int button, int action, int mods)
{
	bool subwindow_has_focus = (get_subwindow_under_mouse() != nullptr);
	bool toolbar_has_focus = (toolbar != nullptr && toolbar->is_mouse_over());
	bool node_has_focus = (view->get_node_under_mouse() != nullptr);
	bool label_has_focus = (view->get_socket_label_under_mouse() != nullptr);
	bool socket_has_focus = (view->get_socket_under_mouse() != nullptr);

	if (subwindow_has_focus) {
		raise_subwindow(get_subwindow_under_mouse());
		get_subwindow_under_mouse()->handle_mouse_button(button, action, mods);
	}

	else if (toolbar_has_focus) {
		toolbar->handle_mouse_button(button, action, mods);
	}

	else if (socket_has_focus) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			view->begin_connection_under_mouse();
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			view->complete_connection_at_mouse();
		}
	}

	else if (node_has_focus) {
		// If the user clicked a label, select it
		if (label_has_focus) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				view->select_label_under_mouse();
			}
		}

		// Move all selected nodes when user clicks the node header
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			if (mods & GLFW_MOD_CONTROL) {
				view->raise_node_under_mouse(SelectMode::ADD);
			}
			else if (mods & GLFW_MOD_SHIFT) {
				view->raise_node_under_mouse(SelectMode::TOGGLE);
			}
			else {
				if (view->is_node_under_mouse_selected()) {
					view->raise_node_under_mouse(SelectMode::NONE);
				}
				else {
					view->raise_node_under_mouse(SelectMode::EXCLUSIVE);
				}
			}
			view->node_move_begin();
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			view->node_move_end();
		}

		view->get_node_under_mouse()->handle_mouse_button(button, action, mods);
	}

	else  { // View
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			view->mouse_move_begin();
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			if (node_list_window->active_button != nullptr) {
				CyclesShaderEditor::Point2 creation_pos(0.0f, 0.0f);
				EditorNode* new_node = node_list_window->active_button->create_node(creation_pos);
				view->add_node_at_mouse(new_node);
				node_list_window->active_button->pressed = false;
				node_list_window->active_button = nullptr;
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			view->deselect_label();
			view->box_select_begin();
		}
	}

	// Handle middle mouse release here so it triggers even if the mouse is over a subwindow
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		view->mouse_move_end();
	}

	// Same for left mouse release, this will cancel/end any action that is currently happening
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// Connection creation
		view->cancel_connection();

		// Node creation
		if (node_list_window != nullptr && node_list_window->active_button != nullptr) {
			node_list_window->active_button->pressed = false;
			node_list_window->active_button = nullptr;
		}

		// Param window
		if (param_editor_window != nullptr) {
			param_editor_window->mouse_button_release();
		}

		// Node moving
		for (EditorNode* node : nodes) {
			node->move_end();
		}

		// Toolbar buttons
		if (toolbar != nullptr) {
			toolbar->release_buttons();
		}

		// Box selection
		SelectMode box_mode = SelectMode::EXCLUSIVE;
		if (mods & GLFW_MOD_CONTROL) {
			box_mode = SelectMode::ADD;
		}
		else if (mods & GLFW_MOD_SHIFT) {
			box_mode = SelectMode::TOGGLE;
		}
		view->box_select_end(box_mode);
	}
}

void CyclesShaderEditor::EditorMainWindow::handle_key(int key, int scancode, int action, int mods)
{
	// System inputs that should be handled with greater priority than anything else
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
			requests.move_left = true;
			return;
		case GLFW_KEY_RIGHT:
			requests.move_right = true;
			return;
		case GLFW_KEY_UP:
			requests.move_up = true;
			return;
		case GLFW_KEY_DOWN:
			requests.move_down = true;
			return;
		default:
			break;
		}
	}
	

	// Normal input handled here
	if (param_editor_window->should_capture_keys()) {
		param_editor_window->handle_key(key, scancode, action, mods);
	}
	else {
		#ifdef __APPLE__
		const int delete_key = GLFW_KEY_BACKSPACE;
		#else
		const int delete_key = GLFW_KEY_DELETE;
		#endif
		if (key == delete_key && action == GLFW_PRESS) {
			view->delete_selected_nodes();
			push_undo_state();
		}
	}
}

void CyclesShaderEditor::EditorMainWindow::handle_character(unsigned int codepoint)
{
	if (param_editor_window->should_capture_keys()) {
		param_editor_window->handle_character(codepoint);
	}
}

void CyclesShaderEditor::EditorMainWindow::handle_scroll(double /*xoffset*/, double yoffset)
{
	if (yoffset > 0.1) {
		requests.zoom_in = true;
	}
	else if (yoffset < -0.1) {
		requests.zoom_out = true;
	}
}

void CyclesShaderEditor::EditorMainWindow::load_serialized_graph(std::string graph)
{
	clear_graph(true);
	deserialize_graph(graph, nodes, connections);
	update_serialized_state();
}

void CyclesShaderEditor::EditorMainWindow::pre_draw()
{
	// Check nodes to see if we should save current state
	bool should_push_undo_state = false;
	for (EditorNode* node : nodes) {
		if (node->changed) {
			should_push_undo_state = true;
			node->changed = false;
		}
	}
	if (param_editor_window->should_push_undo_state()) {
		should_push_undo_state = true;
	}
	if (should_push_undo_state) {
		push_undo_state();
	}

	// Update mouse position
	double mx, my;
	glfwGetCursorPos(window, &mx, &my);
	glfwGetWindowSize(window, &window_width, &window_height);

	update_mouse_position(CyclesShaderEditor::Point2(static_cast<float>(mx), static_cast<float>(my)));
	screen_to_world = CyclesShaderEditor::Point2(view_center.get_pos_x() - window_width / 2, view_center.get_pos_y() - window_height / 2 - UI_TOOLBAR_HEIGHT / 2.0f);

	// Handle window events
	glfwPollEvents();

	// Handle internal requests
	service_requests();

	// Update any other state we need
	if (param_editor_window != nullptr) {
		param_editor_window->set_selected_param(view->get_selected_socket_label());
	}
	view->update(mouse_screen_pos, window_width, window_height);
	status_bar->set_zoom_text(view->get_zoom_string());

	// Mark all connected input sockets
	for (NodeConnection& connection : connections) {
		connection.end_socket->input_connected_this_frame = true;
	}

	for (NodeEditorSubwindow* this_subwindow : subwindows) {
		this_subwindow->pre_draw();
	}
}

void CyclesShaderEditor::EditorMainWindow::draw()
{
	//////
	// Update state here
	//////

	// Update toolbar button state
	if (undo_stack.undo_available()) {
		toolbar->enable_button(CyclesShaderEditor::ToolbarButtonType::UNDO);
	}
	else {
		toolbar->disable_button(CyclesShaderEditor::ToolbarButtonType::UNDO);
	}
	if (undo_stack.redo_available()) {
		toolbar->enable_button(CyclesShaderEditor::ToolbarButtonType::REDO);
	}
	else {
		toolbar->disable_button(CyclesShaderEditor::ToolbarButtonType::REDO);
	}

	//////
	// Draw here
	//////

	nvgSave(nvg_context);
	view->draw(nvg_context);
	nvgRestore(nvg_context);

	// Draw toolbar
	if (toolbar != nullptr) {
		toolbar->set_mouse_position(mouse_screen_pos);
		toolbar->draw(nvg_context, static_cast<float>(window_width));
	}

	// Draw subwindows
	const float max_safe_pos_y = window_height - UI_STATUSBAR_HEIGHT;
	std::list<NodeEditorSubwindow*>::reverse_iterator window_iter;
	for (window_iter = subwindows.rbegin(); window_iter != subwindows.rend(); ++window_iter) {
		const CyclesShaderEditor::Point2 subwindow_pos = (*window_iter)->get_screen_pos();
		const CyclesShaderEditor::Point2 local_mouse_pos = mouse_screen_pos - subwindow_pos;
		(*window_iter)->set_mouse_position(local_mouse_pos, max_safe_pos_y);
		nvgSave(nvg_context);
		nvgTranslate(nvg_context, subwindow_pos.get_pos_x(), subwindow_pos.get_pos_y());
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
				thread_usleep(sleep_time_us);
			}
		}
	}
	last_buffer_swap_time = std::chrono::steady_clock::now();
	glfwSwapBuffers(window);
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
	if (requests.move_left) {
		view->move_left();
		requests.move_left = false;
	}
	if (requests.move_right) {
		view->move_right();
		requests.move_right = false;
	}
	if (requests.move_up) {
		view->move_up();
		requests.move_up = false;
	}
	if (requests.move_down) {
		view->move_down();
		requests.move_down = false;
	}
	if (requests.zoom_in) {
		view->zoom_in();
		requests.zoom_in = false;
	}
	if (requests.zoom_out) {
		view->zoom_out();
		requests.zoom_out = false;
	}
}

void CyclesShaderEditor::EditorMainWindow::update_mouse_position(CyclesShaderEditor::Point2 screen_position)
{
	screen_position.clamp_to(CyclesShaderEditor::Point2(0.0f, 0.0f), CyclesShaderEditor::Point2(window_width - 1.0f, window_height - 1.0f));
	mouse_screen_pos = screen_position;
}

CyclesShaderEditor::NodeEditorSubwindow* CyclesShaderEditor::EditorMainWindow::get_subwindow_under_mouse()
{
	for (NodeEditorSubwindow* this_subwindow : subwindows) {
		if (this_subwindow->is_mouse_over()) {
			return this_subwindow;
		}
	}
	return nullptr;
}

void CyclesShaderEditor::EditorMainWindow::raise_subwindow(NodeEditorSubwindow* subwindow)
{
	std::list<NodeEditorSubwindow*>::iterator subwindow_iter;
	for (subwindow_iter = subwindows.begin(); subwindow_iter != subwindows.end(); ++subwindow_iter) {
		NodeEditorSubwindow* this_window = *subwindow_iter;
		if (this_window == subwindow) {
			subwindows.erase(subwindow_iter);
			subwindows.push_front(this_window);
			return;
		}
	}
}

void CyclesShaderEditor::EditorMainWindow::update_serialized_state()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;
	generate_output_lists(nodes, connections, out_nodes, out_connections);
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
	std::string new_state = undo_stack.pop_undo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, nodes, connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void CyclesShaderEditor::EditorMainWindow::redo()
{
	if (undo_stack.redo_available() == false) {
		return;
	}
	update_serialized_state();
	std::string new_state = undo_stack.pop_redo_state(serialized_state);
	clear_graph(false);
	deserialize_graph(new_state, nodes, connections);
	update_serialized_state();
	status_bar->set_status_text("Graph contains unsaved changes");
}

void CyclesShaderEditor::EditorMainWindow::clear_graph(bool reset_undo)
{
	if (reset_undo) {
		undo_stack.clear();
	}
	view->deselect_label();
	view->clear_node_selection();
	nodes.clear();
	connections.clear();
	update_serialized_state();
}

void CyclesShaderEditor::EditorMainWindow::do_output()
{
	std::vector<OutputNode> out_nodes;
	std::vector<OutputConnection> out_connections;

	generate_output_lists(nodes, connections, out_nodes, out_connections);

	clear_graph(false);

	public_window->serialized_output = serialize_graph(out_nodes, out_connections);

	// Re-create graph from saved state so serialization errors are more apparent
	deserialize_graph(public_window->serialized_output, nodes, connections);
	update_serialized_state();

	public_window->output_updated = true;
	status_bar->set_status_text("Saved");
}

void CyclesShaderEditor::EditorMainWindow::release_resources()
{
	for (NodeEditorSubwindow* this_subwindow : subwindows) {
		delete this_subwindow;
	}
	subwindows.clear();
	node_list_window = nullptr;
	param_editor_window = nullptr;

	connections.clear();
	for (EditorNode* this_node : nodes) {
		delete this_node;
	}
	nodes.clear();

	if (toolbar != nullptr) {
		delete toolbar;
		toolbar = nullptr;
	}
	if (status_bar != nullptr) {
		delete status_bar;
		status_bar = nullptr;
	}

	if (view != nullptr) {
		delete view;
		view = nullptr;
	}

	if (window != nullptr) {
		(*get_callback_window_map()).erase(window);
		glfwDestroyWindow(window);
		window = nullptr;
	}

	if (nvg_context != nullptr) {
		nvgDeleteGL2(nvg_context);
		nvg_context = nullptr;
	}

	glfwTerminate();
}