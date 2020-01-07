#include "view.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <list>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "editable_graph.h"
#include "node_base.h"
#include "selection.h"
#include "sockets.h"
#include "subwindow_node_list.h"
#include "ui_requests.h"
#include "util_area.h"
#include "util_enum.h"
#include "util_platform.h"

static constexpr int GRID_SIZE_INT = 32;
static constexpr float GRID_SIZE_FL = static_cast<float>(GRID_SIZE_INT);

cse::EditGraphView::ViewBorders::ViewBorders(
	const Float2 view_center,
	const int viewport_width,
	const int viewport_height,
	const float zoom_scale
	) :
	left  (view_center.x - static_cast<int>(viewport_width / zoom_scale) / 2),
	right (view_center.x + static_cast<int>(viewport_width / zoom_scale) / 2),
	top   (view_center.y - static_cast<int>(viewport_height / zoom_scale) / 2),
	bottom(view_center.y + static_cast<int>(viewport_height / zoom_scale) / 2)
{

}

cse::EditGraphView::EditGraphView(const std::shared_ptr<EditableGraph> graph, const std::weak_ptr<NodeCreationHelper> node_creation_helper)
	: graph(graph), node_creation_helper(node_creation_helper), selection(std::make_shared<Selection>())
{
	view_center = Float2(0.0f, 0.0f);
}

void cse::EditGraphView::set_mouse_position(const Float2 view_local_mouse_pos, const int viewport_width, const int viewport_height)
{
	widget_width = viewport_width;
	widget_height = viewport_height;

	const float zoom_scale = zoom_level.get_world_scale();
	const ViewBorders borders(view_center, widget_width, widget_height, zoom_scale);

	// Calculate the viewport height and width, in world space
	const float view_width = viewport_width / zoom_scale;
	const float view_height = viewport_height / zoom_scale;

	// Convert mouse position from view space to world space
	const float x_pos = view_local_mouse_pos.x / (viewport_width) * (view_width) + borders.left;
	const float y_pos = view_local_mouse_pos.y / (viewport_height) * (view_height) + borders.top;

	const Float2 calculated_new_world_pos(x_pos, y_pos);
	const Float2 mouse_delta = calculated_new_world_pos - mouse_world_position;

	// If we are panning the view, adjust the view center so the mouse stays in the same world position
	if (mouse_pan_active) {
		view_center = view_center - mouse_delta;
	}

	if (node_move_active) {
		if (mouse_delta.is_nonzero()) {
			selection->move_nodes(mouse_delta);
			node_move_did_something = true;
		}
	}

	// Mouse stays anchored in the world during a pan
	if (mouse_pan_active == false) {
		mouse_world_position = calculated_new_world_pos;
	}
}

void cse::EditGraphView::update()
{
	if (box_select_active) {
		world_box_select_end = mouse_world_position;
	}
}

void cse::EditGraphView::draw(NVGcontext* draw_context)
{
	const float zoom_scale = zoom_level.get_world_scale();
	const ViewBorders borders(view_center, widget_width, widget_height, zoom_scale);

	// Translate the origin so we are drawing in world space
	nvgTranslate(draw_context, -1 * borders.left * zoom_scale, -1 * borders.top * zoom_scale);
	nvgScale(draw_context, zoom_scale, zoom_scale);

	// Draw lightest grid
	{
		nvgBeginPath(draw_context);
		nvgStrokeWidth(draw_context, 1.2f);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 0.28f));

		// Horizontal
		float next_line_y = floor(borders.top / GRID_SIZE_FL) * GRID_SIZE_FL;
		while (next_line_y < borders.bottom) {
			nvgMoveTo(draw_context, borders.left, next_line_y);
			nvgLineTo(draw_context, borders.right, next_line_y);
			next_line_y += GRID_SIZE_FL;
		}
		// Vertical
		float next_line_x = floor(borders.left / GRID_SIZE_FL) * GRID_SIZE_FL;
		while (next_line_x < borders.right) {
			nvgMoveTo(draw_context, next_line_x, borders.top);
			nvgLineTo(draw_context, next_line_x, borders.bottom);
			next_line_x += GRID_SIZE_FL;
		}

		nvgStroke(draw_context);
	}

	// Medium darkness lines
	// Same as above but with 8 * grid size
	{
		const float MEDIUM_GRID_SIZE = GRID_SIZE_FL * 8;
		nvgBeginPath(draw_context);
		nvgStrokeWidth(draw_context, 1.8f);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 0.45f));

		// Horizontal
		float next_line_y = floor(borders.top / MEDIUM_GRID_SIZE) * MEDIUM_GRID_SIZE;
		while (next_line_y < borders.bottom) {
			nvgMoveTo(draw_context, borders.left, next_line_y);
			nvgLineTo(draw_context, borders.right, next_line_y);
			next_line_y += MEDIUM_GRID_SIZE;
		}
		// Vertical
		float next_line_x = floor(borders.left / MEDIUM_GRID_SIZE) * MEDIUM_GRID_SIZE;
		while (next_line_x < borders.right) {
			nvgMoveTo(draw_context, next_line_x, borders.top);
			nvgLineTo(draw_context, next_line_x, borders.bottom);
			next_line_x += MEDIUM_GRID_SIZE;
		}

		nvgStroke(draw_context);
	}

	// Dark lines at the origin
	nvgStrokeWidth(draw_context, 2.75f);
	nvgStrokeColor(draw_context, nvgRGBf(0.0f, 0.0f, 0.0f));

	nvgBeginPath(draw_context);

	nvgMoveTo(draw_context, 0.0f, borders.top);
	nvgLineTo(draw_context, 0.0f, borders.bottom);

	nvgMoveTo(draw_context, borders.left, 0.0f);
	nvgLineTo(draw_context, borders.right, 0.0f);

	nvgStroke(draw_context);

	// Nodes
	const std::shared_ptr<NodeSocket> selected_node = selection->socket.lock();
	for (auto node_iterator = graph->nodes.rbegin(); node_iterator != graph->nodes.rend(); ++node_iterator) {
		const std::shared_ptr<EditableNode> this_node = *node_iterator;
		nvgSave(draw_context);
		const float x = std::floor(this_node->world_pos.x);
		const float y = std::floor(this_node->world_pos.y);
		nvgTranslate(draw_context, x, y);
		const bool node_selected = (selection->nodes.count(this_node) == 1);
		this_node->draw_node(draw_context, node_selected, selected_node);
		nvgRestore(draw_context);
	}

	// Box selection indicator
	if (box_select_active) {
		nvgStrokeWidth(draw_context, 1.0f / zoom_scale); // Make this 1 pixel wide, regardless of zoom
		nvgStrokeColor(draw_context, nvgRGBf(1.0f, 1.0f, 1.0f));

		nvgBeginPath(draw_context);

		nvgMoveTo(draw_context, world_box_select_begin.x, world_box_select_begin.y);
		nvgLineTo(draw_context, world_box_select_begin.x, world_box_select_end.y);
		nvgLineTo(draw_context, world_box_select_end.x, world_box_select_end.y);
		nvgLineTo(draw_context, world_box_select_end.x, world_box_select_begin.y);
		nvgLineTo(draw_context, world_box_select_begin.x, world_box_select_begin.y);

		nvgStroke(draw_context);
	}

	// Connections
	std::list<NodeConnection>::iterator connection_iter;
	for (connection_iter = graph->connections.begin(); connection_iter != graph->connections.end(); ++connection_iter) {
		NodeConnection this_connection = *connection_iter;
		auto conn_begin = this_connection.begin_socket.lock();
		auto conn_end = this_connection.end_socket.lock();
		if (conn_begin && conn_end) {
			Drawing::draw_node_connection_curve(draw_context, conn_begin->world_draw_position, conn_end->world_draw_position, 2.0f);
		}
	}

	// Connection in progress
	if (const auto connection_start = connection_in_progress_start.lock()) {
		Drawing::draw_node_connection_curve(draw_context, connection_start->world_draw_position, mouse_world_position, 3.2f);
	}

	nvgRestore(draw_context);
}

void cse::EditGraphView::handle_mouse_button(const int button, const int action, const int mods)
{
	const std::weak_ptr<EditableNode> weak_focused_node = graph->get_node_under_point(mouse_world_position);
	const auto focused_label_ptr = graph->get_socket_under_point(mouse_world_position).lock();
	const auto focused_connector_in_ptr = graph->get_connector_under_point(mouse_world_position, SocketIOType::INPUT).lock();
	const auto focused_connector_out_ptr = graph->get_connector_under_point(mouse_world_position, SocketIOType::OUTPUT).lock();

	if (focused_connector_out_ptr) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			begin_connection(focused_connector_out_ptr);
		}
	}
	else if (focused_connector_in_ptr) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			reroute_connection(focused_connector_in_ptr);
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			complete_connection(focused_connector_in_ptr);
		}
	}
	else if (const auto focused_node = weak_focused_node.lock()) {
		// If the user clicked a label, select it
		if (focused_label_ptr) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				select_label_under_mouse();
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			// Raise the node under the mouse and add it to the selection, then begin moving entire selection
			const std::weak_ptr<EditableNode> node_to_select = graph->get_node_under_point(mouse_world_position);
			graph->raise_node(node_to_select);
			if (mods == GLFW_MOD_CONTROL) {
				selection->modify_selection(SelectMode::ADD, node_to_select);
			}
			else if (mods == GLFW_MOD_SHIFT) {
				selection->modify_selection(SelectMode::TOGGLE, node_to_select);
			}
			else {
				selection->modify_selection(SelectMode::NORMAL, node_to_select);
			}
			node_move_begin();
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			node_move_end();
		}
	}
	else { // Nothing specific has focus
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			deselect_label();
			box_select_begin();
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			if (const auto node_creation_helper = this->node_creation_helper.lock()) {
				std::shared_ptr<EditableNode> new_node = node_creation_helper->take();
				if (new_node) {
					graph->add_node(new_node, mouse_world_position);
					selection->nodes.clear();
				}
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			mouse_pan_active = true;
		}
	}

	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			// End connection in progress
			cancel_connection();

			// Box selection
			SelectMode box_mode = SelectMode::NORMAL;
			if (mods & GLFW_MOD_CONTROL) {
				box_mode = SelectMode::ADD;
			}
			else if (mods & GLFW_MOD_SHIFT) {
				box_mode = SelectMode::TOGGLE;
			}
			box_select_end(box_mode);
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			mouse_pan_active = false;
		}
	}
}

void cse::EditGraphView::handle_key(const int key, int /*scancode*/, const int action, int /*mods*/)
{
	const int delete_key = Platform::get_delete_key();
	if (key == delete_key && action == GLFW_PRESS) {
		graph->remove_node_set(selection->nodes);
	}
}

void cse::EditGraphView::handle_requests(ViewUIRequests& requests)
{
	if (requests.pan_left) {
		pan(-1, 0);
		requests.pan_left = false;
	}
	if (requests.pan_right) {
		pan(1, 0);
		requests.pan_right = false;
	}
	if (requests.pan_up) {
		pan(0, -1);
		requests.pan_up = false;
	}
	if (requests.pan_down) {
		pan(0, 1);
		requests.pan_down = false;
	}

	if (requests.zoom_in) {
		zoom_level.zoom_in();
		snap_view_center();
		requests.zoom_in = false;
	}
	if (requests.zoom_out) {
		zoom_level.zoom_out();
		snap_view_center();
		requests.zoom_out = false;
	}
}

bool cse::EditGraphView::needs_undo_push()
{
	bool result = false;
	if (should_push_undo_state) {
		should_push_undo_state = false;
		result = true;
	}
	return result;
}

std::weak_ptr<const cse::Selection> cse::EditGraphView::get_const_selection() const
{
	return selection;
}

void cse::EditGraphView::clear_selection()
{
	selection->clear();
}

std::string cse::EditGraphView::get_zoom_string() const
{
	constexpr unsigned char BUFFER_SIZE = 24;
	char buf[BUFFER_SIZE];
	const int result = snprintf(buf, BUFFER_SIZE, "Zoom: %3.1f%%", zoom_level.get_world_scale() * 100.0f);
	if (result <= 0) {
		// This should never happen
		return "ZOOM ERROR";
	}
	return std::string(buf);
}

void cse::EditGraphView::begin_connection(const std::weak_ptr<NodeSocket> socket_begin)
{
	if (const auto socket_begin_ptr = socket_begin.lock()) {
		if (socket_begin_ptr->io_type == SocketIOType::OUTPUT) {
			connection_in_progress_start = socket_begin;
			return;
		}
	}
	// If no connection was started, clear the current connection-in-progress state
	cancel_connection();
}

void cse::EditGraphView::complete_connection(const std::weak_ptr<NodeSocket> socket_end)
{
	if (connection_in_progress_start.expired()) {
		return;
	}
	if (socket_end.lock()) {
		graph->add_connection(connection_in_progress_start, socket_end);
	}
	cancel_connection();
}

void cse::EditGraphView::reroute_connection(const std::weak_ptr<NodeSocket> socket_end)
{
	NodeConnection removed_connection = graph->remove_connection_with_end(socket_end);
	connection_in_progress_start = removed_connection.begin_socket;
}

void cse::EditGraphView::cancel_connection()
{
	connection_in_progress_start = std::weak_ptr<NodeSocket>();
}

void cse::EditGraphView::select_label(const std::weak_ptr<NodeSocket> label)
{
	const std::shared_ptr<NodeSocket> selected_socket_ptr = selection->socket.lock();
	if (selected_socket_ptr == label.lock()) {
		return;
	}
	selection->socket = label;
}

cse::WeakNodeSet cse::EditGraphView::get_boxed_nodes()
{
	WeakNodeSet result;

	float min_x_pos = 0.0f;
	float min_y_pos = 0.0f;
	float max_x_pos = 0.0f;
	float max_y_pos = 0.0f;

	Area select_box = Area(world_box_select_begin, world_box_select_end);

	for (const auto& this_node : graph->nodes) {
		const Area node_box = Area(this_node->world_pos, this_node->world_pos + this_node->get_dimensions());
		if (select_box.overlaps(node_box)) {
			result.insert(this_node);
		}
	}

	return result;
}

void cse::EditGraphView::node_move_begin()
{
	node_move_active = true;
	node_move_did_something = false;
}

void cse::EditGraphView::node_move_end()
{
	if (node_move_active && node_move_did_something) {
		should_push_undo_state = true;
	}
	node_move_active = false;
	node_move_did_something = false;
}

void cse::EditGraphView::pan(const int horizontal_ticks, const int vertical_ticks)
{
	view_center = view_center + Float2(GRID_SIZE_FL * horizontal_ticks, GRID_SIZE_FL * vertical_ticks);
}

void cse::EditGraphView::snap_view_center()
{
	// Snap view to nearest whole number (x, y) coordinate
	// This is to avoid blurry text and lines after a user has panned around while zoomed in
	const float x = std::roundf(view_center.x);
	const float y = std::roundf(view_center.y);
	view_center = Float2(x, y);
}

void cse::EditGraphView::box_select_begin()
{
	box_select_active = true;
	world_box_select_begin = mouse_world_position;
	world_box_select_end = mouse_world_position;
}

void cse::EditGraphView::box_select_end(SelectMode mode)
{
	if (box_select_active == false) {
		return;
	}

	WeakNodeSet boxed_nodes = get_boxed_nodes();

	switch (mode) {
	case SelectMode::NORMAL:
		selection->nodes.clear();
		selection->nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
		break;
	case SelectMode::ADD:
		selection->nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
		break;
	case SelectMode::TOGGLE:
		for (const auto& this_node : boxed_nodes) {
			if (selection->nodes.count(this_node) == 1) {
				selection->nodes.erase(this_node);
			}
			else {
				selection->nodes.insert(this_node);
			}
		}
		break;
	}

	box_select_active = false;
}

void cse::EditGraphView::select_label_under_mouse()
{
	select_label(graph->get_socket_under_point(mouse_world_position));
}

void cse::EditGraphView::deselect_label()
{
	select_label(std::weak_ptr<NodeSocket>());
}
