#include "view.h"

#include <cassert>
#include <cmath>
#include <cstdio>

#include <nanovg.h>

#include "util_rectangle.h"

#include <iostream>

static constexpr int GRID_SIZE_INT = 32;
static constexpr float GRID_SIZE_FL = static_cast<float>(GRID_SIZE_INT);

CyclesShaderEditor::EditGraphView::EditGraphView(
		std::list<EditorNode*>& nodes,
		std::list<NodeConnection>& connections
	) :
	nodes(nodes),
	connections(connections)
{
	view_center = Point2(0.0f, 0.0f);
}

CyclesShaderEditor::EditorNode* CyclesShaderEditor::EditGraphView::get_node_under_mouse()
{
	for (EditorNode* this_node : nodes) {
		if (this_node->is_mouse_over_node()) {
			return this_node;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_socket_label_under_mouse()
{
	for (EditorNode* this_node : nodes) {
		NodeSocket* maybe_socket = this_node->get_socket_label_under_mouse();
		if (maybe_socket != nullptr) {
			return maybe_socket;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_selected_socket_label()
{
	return selected_label;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_socket_under_mouse()
{
	for (EditorNode* this_node : nodes) {
		NodeSocket* maybe_socket = this_node->get_socket_under_mouse();
		if (maybe_socket != nullptr) {
			return maybe_socket;
		}
	}

	return nullptr;
}

CyclesShaderEditor::Point2 CyclesShaderEditor::EditGraphView::get_mouse_world_position()
{
	return mouse_world_position;
}

bool CyclesShaderEditor::EditGraphView::is_node_under_mouse_selected()
{
	if (selected_nodes.empty()) {
		return false;
	}

	EditorNode* const under_mouse = get_node_under_mouse();
	if (under_mouse == nullptr) {
		return false;
	}

	return (selected_nodes.count(under_mouse) == 1);
}

std::string CyclesShaderEditor::EditGraphView::get_zoom_string()
{
	char buf[20];
	snprintf(buf, 20, "Zoom: %3.1f%%", zoom.get_world_scale() * 100.0f);
	return std::string(buf);
}

void CyclesShaderEditor::EditGraphView::update(Point2 view_local_mouse_pos, int viewport_width, int viewport_height)
{
	const float zoom_scale = zoom.get_world_scale();

	// Calculate the viewport height and width, in world space
	const float view_width = viewport_width / zoom_scale;
	const float view_height = viewport_height / zoom_scale;

	// Calculate screen limits
	border_left = view_center.get_floor_pos_x() - static_cast<int>(view_width) / 2;
	border_right = view_center.get_floor_pos_x() + static_cast<int>(view_width) / 2;
	border_top = view_center.get_floor_pos_y() - static_cast<int>(view_height) / 2;
	border_bottom = view_center.get_floor_pos_y() + static_cast<int>(view_height) / 2;

	if (mouse_move_active) {
		// Find the difference between where the mouse was last frame, and where it will be this frame
		const float x_pos = view_local_mouse_pos.get_pos_x() / (viewport_width) * (view_width)+border_left;
		const float y_pos = view_local_mouse_pos.get_pos_y() / (viewport_height) * (view_height)+border_top;
		Point2 potential_world_position(x_pos, y_pos);
		Point2 delta = potential_world_position - mouse_world_position;
		view_center = view_center - delta;
		// Re-calculate screen limits with new center
		border_left = view_center.get_floor_pos_x() - static_cast<int>(view_width) / 2;
		border_right = view_center.get_floor_pos_x() + static_cast<int>(view_width) / 2;
		border_top = view_center.get_floor_pos_y() - static_cast<int>(view_height) / 2;
		border_bottom = view_center.get_floor_pos_y() + static_cast<int>(view_height) / 2;
	}
	// Convert coordinate from window space to world space
	const float x_pos = view_local_mouse_pos.get_pos_x() / (viewport_width) * (view_width) + border_left;
	const float y_pos = view_local_mouse_pos.get_pos_y() / (viewport_height) * (view_height) + border_top;
	mouse_world_position = Point2(x_pos, y_pos);

	if (box_select_active) {
		world_box_select_end = mouse_world_position;
	}

	// Iterate through nodes to set the 'selected' state for each
	for (EditorNode* this_node : nodes) {
		if (selected_nodes.count(this_node) == 1) {
			this_node->selected = true;
		}
		else {
			this_node->selected = false;
		}
	}
}

void CyclesShaderEditor::EditGraphView::draw(NVGcontext* draw_context)
{
	const float zoom_scale = zoom.get_world_scale();
	// Translate the origin so we are drawing in world space
	nvgTranslate(draw_context, -1 * border_left * zoom_scale, -1 * border_top * zoom_scale);
	nvgScale(draw_context, zoom_scale, zoom_scale);

	// Draw lightest grid
	{
		nvgBeginPath(draw_context);
		nvgStrokeWidth(draw_context, 1.2f);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 0.28f));

		// Horizontal
		float next_line_y = floor(border_top / GRID_SIZE_FL) * GRID_SIZE_FL;
		while (next_line_y < border_bottom) {
			nvgMoveTo(draw_context, border_left, next_line_y);
			nvgLineTo(draw_context, border_right, next_line_y);
			next_line_y += GRID_SIZE_FL;
		}
		// Vertical
		float next_line_x = floor(border_left / GRID_SIZE_FL) * GRID_SIZE_FL;
		while (next_line_x < border_right) {
			nvgMoveTo(draw_context, next_line_x, border_top);
			nvgLineTo(draw_context, next_line_x, border_bottom);
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
		float next_line_y = floor(border_top / MEDIUM_GRID_SIZE) * MEDIUM_GRID_SIZE;
		while (next_line_y < border_bottom) {
			nvgMoveTo(draw_context, border_left, next_line_y);
			nvgLineTo(draw_context, border_right, next_line_y);
			next_line_y += MEDIUM_GRID_SIZE;
		}
		// Vertical
		float next_line_x = floor(border_left / MEDIUM_GRID_SIZE) * MEDIUM_GRID_SIZE;
		while (next_line_x < border_right) {
			nvgMoveTo(draw_context, next_line_x, border_top);
			nvgLineTo(draw_context, next_line_x, border_bottom);
			next_line_x += MEDIUM_GRID_SIZE;
		}

		nvgStroke(draw_context);
	}

	// Dark lines at the origin
	nvgStrokeWidth(draw_context, 2.75f);
	nvgStrokeColor(draw_context, nvgRGBf(0.0f, 0.0f, 0.0f));

	nvgBeginPath(draw_context);

	nvgMoveTo(draw_context, 0.0f, border_top);
	nvgLineTo(draw_context, 0.0f, border_bottom);

	nvgMoveTo(draw_context, border_left, 0.0f);
	nvgLineTo(draw_context, border_right, 0.0f);

	nvgStroke(draw_context);

	// Nodes
	std::list<EditorNode*>::reverse_iterator node_iterator;
	for (node_iterator = nodes.rbegin(); node_iterator != nodes.rend(); ++node_iterator) {
		CyclesShaderEditor::Point2 node_local_mouse_pos = mouse_world_position - (*node_iterator)->world_pos;
		(*node_iterator)->set_mouse_position(node_local_mouse_pos);
		nvgSave(draw_context);
		nvgTranslate(draw_context, (*node_iterator)->world_pos.get_floor_pos_x(), (*node_iterator)->world_pos.get_floor_pos_y());
		(*node_iterator)->draw_node(draw_context);
		nvgRestore(draw_context);
	}

	// Box selection indicator
	if (box_select_active) {
		nvgStrokeWidth(draw_context, 1.0f / zoom_scale); // Make this 1 pixel wide, regardless of zoom
		nvgStrokeColor(draw_context, nvgRGBf(1.0f, 1.0f, 1.0f));

		nvgBeginPath(draw_context);

		nvgMoveTo(draw_context, world_box_select_begin.get_pos_x(), world_box_select_begin.get_pos_y());
		nvgLineTo(draw_context, world_box_select_begin.get_pos_x(), world_box_select_end.get_pos_y());
		nvgLineTo(draw_context, world_box_select_end.get_pos_x(), world_box_select_end.get_pos_y());
		nvgLineTo(draw_context, world_box_select_end.get_pos_x(), world_box_select_begin.get_pos_y());
		nvgLineTo(draw_context, world_box_select_begin.get_pos_x(), world_box_select_begin.get_pos_y());

		nvgStroke(draw_context);
	}

	// Connections
	std::list<NodeConnection>::iterator connection_iter;
	for (connection_iter = connections.begin(); connection_iter != connections.end(); ++connection_iter) {
		NodeConnection this_connection = *connection_iter;

		float source_x = this_connection.begin_socket->world_draw_position.get_pos_x();
		float source_y = this_connection.begin_socket->world_draw_position.get_pos_y();
		float dest_x = this_connection.end_socket->world_draw_position.get_pos_x();
		float dest_y = this_connection.end_socket->world_draw_position.get_pos_y();

		float mid_x = source_x + (dest_x - source_x) / 2;

		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, source_x, source_y);
		nvgBezierTo(draw_context, mid_x, source_y, mid_x, dest_y, dest_x, dest_y);
		nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 255));
		nvgStrokeWidth(draw_context, 2.0f);
		nvgStroke(draw_context);
	}

	// Connection in progress
	if (connection_in_progress_start != nullptr) {
		float source_x = connection_in_progress_start->world_draw_position.get_pos_x();
		float source_y = connection_in_progress_start->world_draw_position.get_pos_y();
		float dest_x = mouse_world_position.get_pos_x();
		float dest_y = mouse_world_position.get_pos_y();

		float mid_x = source_x + (dest_x - source_x) / 2;

		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, source_x, source_y);
		nvgBezierTo(draw_context, mid_x, source_y, mid_x, dest_y, dest_x, dest_y);
		nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 255));
		nvgStrokeWidth(draw_context, 3.2f);
		nvgStroke(draw_context);
	}

	nvgRestore(draw_context);
}

void CyclesShaderEditor::EditGraphView::add_node_at_mouse(EditorNode* node)
{
	if (node == nullptr) {
		return;
	}

	node->world_pos = mouse_world_position;
	nodes.push_front(node);
	selected_nodes.clear();
	selected_nodes.insert(node);
}

void CyclesShaderEditor::EditGraphView::begin_connection_under_mouse()
{
	NodeSocket* const under_mouse = get_socket_under_mouse();
	if (under_mouse == nullptr) {
		connection_in_progress_start = nullptr;
		return;
	}

	if (under_mouse->socket_in_out == SocketInOut::Output) {
		connection_in_progress_start = under_mouse;
	}
	else if (under_mouse->socket_in_out == SocketInOut::Input) {
		// If a connection already exists, remove it and make a new one with the same source. This also allows drag to disconnect.
		under_mouse->parent->changed = true;
		std::list<NodeConnection>::iterator connection_iter;
		for (connection_iter = connections.begin(); connection_iter != connections.end(); ++connection_iter) {
			if (connection_iter->end_socket == under_mouse) {
				// In this case, remove the existing connection and create a new one in progress with the same source
				connection_in_progress_start = connection_iter->begin_socket;
				connections.erase(connection_iter);
				break;
			}
		}
	}
}

void CyclesShaderEditor::EditGraphView::cancel_connection()
{
	connection_in_progress_start = nullptr;
}

void CyclesShaderEditor::EditGraphView::complete_connection_at_mouse()
{
	if (connection_in_progress_start == nullptr) {
		cancel_connection();
		return;
	}
	NodeSocket* const socket_under_mouse = get_socket_under_mouse();
	if (socket_under_mouse == nullptr) {
		cancel_connection();
		return;
	}
	if (socket_under_mouse->socket_in_out != SocketInOut::Input || connection_in_progress_start->socket_in_out != SocketInOut::Output) {
		cancel_connection();
		return;
	}
	if (connection_in_progress_start->parent == socket_under_mouse->parent) {
		// Two sockets on the same node, do not make a connection
		cancel_connection();
		return;
	}

	// Remove any existing connection at this input
	std::list<NodeConnection>::iterator connection_iter = connections.begin();
	while (connection_iter != connections.end()) {
		if (connection_iter->end_socket == socket_under_mouse) {
			connections.erase(connection_iter++);
		}
		else {
			++connection_iter;
		}
	}

	// Flag as changed so undo state is updated
	connection_in_progress_start->parent->changed = true;
	socket_under_mouse->parent->changed = true;

	NodeConnection new_connection(connection_in_progress_start, socket_under_mouse);
	connections.push_front(new_connection);
	cancel_connection();
}

void CyclesShaderEditor::EditGraphView::clear_input_socket_under_mouse()
{
	NodeSocket* const under_mouse = get_socket_under_mouse();
	if (under_mouse == nullptr || under_mouse->socket_in_out != SocketInOut::Input) {
		return;
	}
	std::list<NodeConnection>::iterator connection_iter;
	for (connection_iter = connections.begin(); connection_iter != connections.end(); ++connection_iter) {
		if (connection_iter->end_socket == under_mouse) {
			connections.erase(connection_iter);
			break;
		}
	}
}

void CyclesShaderEditor::EditGraphView::clear_node_selection()
{
	selected_nodes.clear();
}

void CyclesShaderEditor::EditGraphView::delete_selected_nodes()
{
	if (selected_nodes.empty()) {
		return;
	}
	for (EditorNode* this_node : selected_nodes) {
		delete_node(this_node);
	}
	selected_nodes.clear();
}

void CyclesShaderEditor::EditGraphView::delete_node(EditorNode* const node)
{
	if (node == nullptr) {
		return;
	}

	cancel_connection();
	deselect_label();

	if (node->can_be_deleted() == false) {
		return;
	}
	std::list<EditorNode*>::iterator iter;
	for (iter = nodes.begin(); iter != nodes.end(); ++iter) {
		EditorNode* this_node = *iter;
		if (this_node == node) {
			// Iterate through and erase all connections with this node
			std::list<NodeConnection>::iterator connection_iter = connections.begin();
			while (connection_iter != connections.end()) {
				if (connection_iter->includes_node(this_node)) {
					connections.erase(connection_iter++);
				}
				else {
					++connection_iter;
				}
			}
			// Finally erase node
			nodes.erase(iter);
			delete this_node;
			return;
		}
	}
}

void CyclesShaderEditor::EditGraphView::node_move_begin()
{
	for (EditorNode* this_node : selected_nodes) {
		this_node->move_begin();
	}
}

void CyclesShaderEditor::EditGraphView::node_move_end()
{
	for (EditorNode* this_node : nodes) {
		this_node->move_end();
	}
}

void CyclesShaderEditor::EditGraphView::move_left()
{
	view_center = view_center - Point2(GRID_SIZE_FL, 0.0f);
}

void CyclesShaderEditor::EditGraphView::move_right()
{
	view_center = view_center + Point2(GRID_SIZE_FL, 0.0f);
}

void CyclesShaderEditor::EditGraphView::move_up()
{
	view_center = view_center - Point2(0.0f, GRID_SIZE_FL);
}

void CyclesShaderEditor::EditGraphView::move_down()
{
	view_center = view_center + Point2(0.0f, GRID_SIZE_FL);
}

void CyclesShaderEditor::EditGraphView::zoom_in()
{
	zoom.zoom_in();
}

void CyclesShaderEditor::EditGraphView::zoom_out()
{
	zoom.zoom_out();
}

void CyclesShaderEditor::EditGraphView::mouse_move_begin()
{
	mouse_move_active = true;
}

void CyclesShaderEditor::EditGraphView::mouse_move_end()
{
	mouse_move_active = false;
}

void CyclesShaderEditor::EditGraphView::box_select_begin()
{
	box_select_active = true;
	world_box_select_begin = mouse_world_position;
	world_box_select_end = mouse_world_position;
}

void CyclesShaderEditor::EditGraphView::box_select_end(SelectMode mode)
{
	if (box_select_active == false) {
		return;
	}

	std::set<EditorNode*> boxed_nodes = get_boxed_nodes();

	switch (mode) {
		case SelectMode::EXCLUSIVE:
			selected_nodes.clear();
			selected_nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
			break;
		case SelectMode::ADD:
			selected_nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
			break;
		case SelectMode::TOGGLE:
			for (EditorNode* this_node : boxed_nodes) {
				if (selected_nodes.count(this_node) == 1) {
					selected_nodes.erase(this_node);
				}
				else {
					selected_nodes.insert(this_node);
				}
			}
			break;
		case SelectMode::NONE:
			break;
	}

	box_select_active = false;
}

void CyclesShaderEditor::EditGraphView::select_label_under_mouse()
{
	select_label(get_socket_label_under_mouse());
}

void CyclesShaderEditor::EditGraphView::deselect_label()
{
	select_label(nullptr);
}

void CyclesShaderEditor::EditGraphView::raise_node_under_mouse(SelectMode mode)
{
	raise_node(get_node_under_mouse(), mode);
}

void CyclesShaderEditor::EditGraphView::select_label(NodeSocket* label)
{
	if (selected_label == label) {
		return;
	}
	if (selected_label != nullptr) {
		selected_label->selected = false;
	}
	if (label != nullptr) {
		label->selected = true;
	}
	selected_label = label;
}

void CyclesShaderEditor::EditGraphView::raise_node(EditorNode* node, SelectMode mode)
{
	if (node == nullptr) {
		return;
	}

	for (EditorNode* this_node : nodes) {
		if (this_node == node) {
			nodes.remove(this_node);
			nodes.push_front(this_node);

			if (mode == SelectMode::EXCLUSIVE) {
				selected_nodes.clear();
				selected_nodes.insert(this_node);
			}
			else if (mode == SelectMode::ADD) {
				selected_nodes.insert(this_node);
			}
			else if (mode == SelectMode::TOGGLE) {
				if (selected_nodes.count(this_node) == 1) {
					selected_nodes.erase(this_node);
				}
				else {
					selected_nodes.insert(this_node);
				}
			}
			else if (mode == SelectMode::NONE) {
				// Do nothing, explicitly
			}

			return;
		}
	}
}

std::set<CyclesShaderEditor::EditorNode*> CyclesShaderEditor::EditGraphView::get_boxed_nodes()
{
	std::set<EditorNode*> result;

	if (box_select_active == false) {
		assert(false);
	}

	float min_x_pos = 0.0f;
	float min_y_pos = 0.0f;
	float max_x_pos = 0.0f;
	float max_y_pos = 0.0f;

	if (world_box_select_begin.get_pos_x() < world_box_select_end.get_pos_x()) {
		min_x_pos = world_box_select_begin.get_pos_x();
		max_x_pos = world_box_select_end.get_pos_x();
	}
	else {
		min_x_pos = world_box_select_end.get_pos_x();
		max_x_pos = world_box_select_begin.get_pos_x();
	}
	assert(min_x_pos <= max_x_pos);

	if (world_box_select_begin.get_pos_y() < world_box_select_end.get_pos_y()) {
		min_y_pos = world_box_select_begin.get_pos_y();
		max_y_pos = world_box_select_end.get_pos_y();
	}
	else {
		min_y_pos = world_box_select_end.get_pos_y();
		max_y_pos = world_box_select_begin.get_pos_y();
	}
	assert(min_y_pos <= max_y_pos);

	CyclesShaderEditor::Point2 min_position(min_x_pos, min_y_pos);
	CyclesShaderEditor::Point2 max_position(max_x_pos, max_y_pos);

	for (EditorNode* this_node : nodes) {
		if (do_rectangles_overlap(min_position, max_position, this_node->world_pos, this_node->world_pos + this_node->get_dimensions())) {
			result.insert(this_node);
		}
	}

	return result;
}