#include "view.h"

#include <cassert>
#include <cmath>
#include <cstdio>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "editable_graph.h"
#include "selection.h"
#include "subwindow_node_list.h"
#include "util_platform.h"
#include "util_rectangle.h"

static constexpr int GRID_SIZE_INT = 32;
static constexpr float GRID_SIZE_FL = static_cast<float>(GRID_SIZE_INT);

CyclesShaderEditor::EditGraphView::ViewBorders::ViewBorders(
	const FloatPos view_center,
	const int viewport_width,
	const int viewport_height,
	const float zoom_scale
	) :
	left  (view_center.get_floor_x() - static_cast<int>(viewport_width / zoom_scale) / 2),
	right (view_center.get_floor_x() + static_cast<int>(viewport_width / zoom_scale) / 2),
	top   (view_center.get_floor_y() - static_cast<int>(viewport_height / zoom_scale) / 2),
	bottom(view_center.get_floor_y() + static_cast<int>(viewport_height / zoom_scale) / 2)
{

}

CyclesShaderEditor::EditGraphView::EditGraphView(EditableGraph* const graph, std::weak_ptr<NodeCreationHelper> node_creation_helper)
	: graph(graph), node_creation_helper(node_creation_helper), selection(std::make_shared<Selection>())
{
	view_center = FloatPos(0.0f, 0.0f);
}

CyclesShaderEditor::EditorNode* CyclesShaderEditor::EditGraphView::get_node_under_mouse()
{
	for (EditorNode* this_node : graph->nodes) {
		if (this_node->is_node_under_point(mouse_world_position)) {
			return this_node;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_socket_label_under_mouse()
{
	for (EditorNode* const this_node : graph->nodes) {
		NodeSocket* const maybe_socket = this_node->get_socket_label_under_point(mouse_world_position);
		if (maybe_socket != nullptr) {
			return maybe_socket;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_selected_socket_label()
{
	return selection->socket;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditGraphView::get_socket_connector_under_mouse()
{
	for (EditorNode* this_node : graph->nodes) {
		NodeSocket* maybe_socket = this_node->get_socket_connector_under_point(mouse_world_position);
		if (maybe_socket != nullptr) {
			return maybe_socket;
		}
	}

	return nullptr;
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::EditGraphView::get_mouse_world_position()
{
	return mouse_world_position;
}

bool CyclesShaderEditor::EditGraphView::is_node_under_mouse_selected()
{
	if (selection->nodes.empty()) {
		return false;
	}

	EditorNode* const under_mouse = get_node_under_mouse();
	if (under_mouse == nullptr) {
		return false;
	}

	return (selection->nodes.count(under_mouse) == 1);
}

std::string CyclesShaderEditor::EditGraphView::get_zoom_string()
{
	char buf[20];
	const int result = snprintf(buf, 20, "Zoom: %3.1f%%", zoom.get_world_scale() * 100.0f);
	if (result <= 0) {
		// This should never happen
		return "ZOOM ERROR";
	}
	return std::string(buf);
}

void CyclesShaderEditor::EditGraphView::set_mouse_position(const FloatPos view_local_mouse_pos, const int viewport_width, const int viewport_height)
{
	widget_width = viewport_width;
	widget_height = viewport_height;

	const float zoom_scale = zoom.get_world_scale();
	const ViewBorders borders(view_center, widget_width, widget_height, zoom_scale);

	// Calculate the viewport height and width, in world space
	const float view_width = viewport_width / zoom_scale;
	const float view_height = viewport_height / zoom_scale;

	// Convert mouse position from widget space to world space
	const float x_pos = view_local_mouse_pos.get_x() / (viewport_width) * (view_width) + borders.left;
	const float y_pos = view_local_mouse_pos.get_y() / (viewport_height) * (view_height) + borders.top;

	const FloatPos calculated_world_pos(x_pos, y_pos);
	FloatPos new_world_pos = calculated_world_pos;

	// If we are panning the view, adjust the view center so the mouse stays in the same world position
	if (mouse_pan_active) {
		const FloatPos delta = mouse_world_position - calculated_world_pos;
		view_center = view_center + delta;
		new_world_pos = mouse_world_position;
	}

	if (node_move_active) {
		const FloatPos mouse_delta = calculated_world_pos - mouse_world_position;
		if (mouse_delta.is_nonzero()) {
			selection->move_nodes(mouse_delta);
			node_move_did_something = true;
		}
	}

	mouse_world_position = new_world_pos;
}

void CyclesShaderEditor::EditGraphView::update()
{
	if (box_select_active) {
		world_box_select_end = mouse_world_position;
	}

	// Iterate through nodes to set the 'selected' state for each
	for (EditorNode* this_node : graph->nodes) {
		if (selection->nodes.count(this_node) == 1) {
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
	std::list<EditorNode*>::reverse_iterator node_iterator;
	for (node_iterator = graph->nodes.rbegin(); node_iterator != graph->nodes.rend(); ++node_iterator) {
		nvgSave(draw_context);
		nvgTranslate(draw_context, (*node_iterator)->world_pos.get_floor_x(), (*node_iterator)->world_pos.get_floor_y());
		(*node_iterator)->draw_node(draw_context);
		nvgRestore(draw_context);
	}

	// Box selection indicator
	if (box_select_active) {
		nvgStrokeWidth(draw_context, 1.0f / zoom_scale); // Make this 1 pixel wide, regardless of zoom
		nvgStrokeColor(draw_context, nvgRGBf(1.0f, 1.0f, 1.0f));

		nvgBeginPath(draw_context);

		nvgMoveTo(draw_context, world_box_select_begin.get_x(), world_box_select_begin.get_y());
		nvgLineTo(draw_context, world_box_select_begin.get_x(), world_box_select_end.get_y());
		nvgLineTo(draw_context, world_box_select_end.get_x(), world_box_select_end.get_y());
		nvgLineTo(draw_context, world_box_select_end.get_x(), world_box_select_begin.get_y());
		nvgLineTo(draw_context, world_box_select_begin.get_x(), world_box_select_begin.get_y());

		nvgStroke(draw_context);
	}

	// Connections
	std::list<NodeConnection>::iterator connection_iter;
	for (connection_iter = graph->connections.begin(); connection_iter != graph->connections.end(); ++connection_iter) {
		NodeConnection this_connection = *connection_iter;
		Drawing::draw_node_connection_curve(draw_context, this_connection.begin_socket->world_draw_position, this_connection.end_socket->world_draw_position, 2.0f);
	}

	// Connection in progress
	if (connection_in_progress_start != nullptr) {
		Drawing::draw_node_connection_curve(draw_context, connection_in_progress_start->world_draw_position, mouse_world_position, 3.2f);
	}

	nvgRestore(draw_context);
}

void CyclesShaderEditor::EditGraphView::handle_mouse_button(const int button, const int action, const int mods)
{
	const bool node_has_focus = (get_node_under_mouse() != nullptr);
	const bool label_has_focus = (get_socket_label_under_mouse() != nullptr);
	const bool connector_has_focus = (get_socket_connector_under_mouse() != nullptr);

	if (connector_has_focus) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			begin_connection_under_mouse();
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			complete_connection_at_mouse();
		}
	}
	else if (node_has_focus) {
		// If the user clicked a label, select it
		if (label_has_focus) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				select_label_under_mouse();
			}
		}

		// Move all selected nodes when user clicks the node header
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

			if (mods & GLFW_MOD_CONTROL) {
				raise_node_under_mouse(SelectMode::ADD);
			}
			else if (mods & GLFW_MOD_SHIFT) {
				raise_node_under_mouse(SelectMode::TOGGLE);
			}
			else {
				if (is_node_under_mouse_selected()) {
					raise_node_under_mouse(SelectMode::NONE);
				}
				else {
					raise_node_under_mouse(SelectMode::EXCLUSIVE);
				}
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
				std::unique_ptr<EditorNode> new_node = node_creation_helper->take();
				if (new_node) {
					add_node_at_mouse(new_node);
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
			SelectMode box_mode = SelectMode::EXCLUSIVE;
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

void CyclesShaderEditor::EditGraphView::handle_key(const int key, const int scancode, const int action, const int mods)
{
	const int delete_key = Platform::get_delete_key();
	if (key == delete_key && action == GLFW_PRESS) {
		delete_selected_nodes();
	}
}

bool CyclesShaderEditor::EditGraphView::needs_undo_push()
{
	bool result = false;
	if (should_push_undo_state) {
		should_push_undo_state = false;
		result = true;
	}
	return result;
}

std::weak_ptr<const CyclesShaderEditor::Selection> CyclesShaderEditor::EditGraphView::get_const_selection() const
{
	return selection;
}

void CyclesShaderEditor::EditGraphView::add_node_at_mouse(std::unique_ptr<EditorNode>& node)
{
	if (node.get() == nullptr) {
		return;
	}

	EditorNode* const node_ptr = node.release();
	node_ptr->world_pos = mouse_world_position;
	graph->nodes.push_front(node_ptr);
	selection->nodes.clear();
	selection->nodes.insert(node_ptr);
}

void CyclesShaderEditor::EditGraphView::begin_connection_under_mouse()
{
	NodeSocket* const under_mouse = get_socket_connector_under_mouse();
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
		for (connection_iter = graph->connections.begin(); connection_iter != graph->connections.end(); ++connection_iter) {
			if (connection_iter->end_socket == under_mouse) {
				// In this case, remove the existing connection and create a new one in progress with the same source
				connection_in_progress_start = connection_iter->begin_socket;
				graph->connections.erase(connection_iter);
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
	NodeSocket* const connector_under_mouse = get_socket_connector_under_mouse();
	if (connector_under_mouse == nullptr) {
		cancel_connection();
		return;
	}
	if (connector_under_mouse->socket_in_out != SocketInOut::Input || connection_in_progress_start->socket_in_out != SocketInOut::Output) {
		cancel_connection();
		return;
	}
	if (connection_in_progress_start->parent == connector_under_mouse->parent) {
		// Two sockets on the same node, do not make a connection
		cancel_connection();
		return;
	}

	// Remove any existing connection at this input
	std::list<NodeConnection>::iterator connection_iter = graph->connections.begin();
	while (connection_iter != graph->connections.end()) {
		if (connection_iter->end_socket == connector_under_mouse) {
			graph->connections.erase(connection_iter++);
		}
		else {
			++connection_iter;
		}
	}

	// Flag as changed so undo state is updated
	connection_in_progress_start->parent->changed = true;
	connector_under_mouse->parent->changed = true;

	NodeConnection new_connection(connection_in_progress_start, connector_under_mouse);
	graph->connections.push_front(new_connection);
	cancel_connection();
}

void CyclesShaderEditor::EditGraphView::clear_input_socket_under_mouse()
{
	NodeSocket* const under_mouse = get_socket_connector_under_mouse();
	if (under_mouse == nullptr || under_mouse->socket_in_out != SocketInOut::Input) {
		return;
	}
	std::list<NodeConnection>::iterator connection_iter;
	for (connection_iter = graph->connections.begin(); connection_iter != graph->connections.end(); ++connection_iter) {
		if (connection_iter->end_socket == under_mouse) {
			graph->connections.erase(connection_iter);
			break;
		}
	}
}

void CyclesShaderEditor::EditGraphView::clear_node_selection()
{
	selection->nodes.clear();
}

void CyclesShaderEditor::EditGraphView::delete_selected_nodes()
{
	if (selection->nodes.empty()) {
		return;
	}
	for (EditorNode* this_node : selection->nodes) {
		delete_node(this_node);
	}
	selection->nodes.clear();
	should_push_undo_state = true;
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
	for (iter = graph->nodes.begin(); iter != graph->nodes.end(); ++iter) {
		EditorNode* this_node = *iter;
		if (this_node == node) {
			// Iterate through and erase all connections with this node
			std::list<NodeConnection>::iterator connection_iter = graph->connections.begin();
			while (connection_iter != graph->connections.end()) {
				if (connection_iter->includes_node(this_node)) {
					graph->connections.erase(connection_iter++);
				}
				else {
					++connection_iter;
				}
			}
			// Finally erase node
			graph->nodes.erase(iter);
			delete this_node;
			return;
		}
	}
}

void CyclesShaderEditor::EditGraphView::node_move_begin()
{
	node_move_active = true;
	node_move_did_something = false;
}

void CyclesShaderEditor::EditGraphView::node_move_end()
{
	if (node_move_active && node_move_did_something) {
		should_push_undo_state = true;
	}
	node_move_active = false;
	node_move_did_something = false;
}

void CyclesShaderEditor::EditGraphView::move_left()
{
	view_center = view_center - FloatPos(GRID_SIZE_FL, 0.0f);
}

void CyclesShaderEditor::EditGraphView::move_right()
{
	view_center = view_center + FloatPos(GRID_SIZE_FL, 0.0f);
}

void CyclesShaderEditor::EditGraphView::move_up()
{
	view_center = view_center - FloatPos(0.0f, GRID_SIZE_FL);
}

void CyclesShaderEditor::EditGraphView::move_down()
{
	view_center = view_center + FloatPos(0.0f, GRID_SIZE_FL);
}

void CyclesShaderEditor::EditGraphView::zoom_in()
{
	zoom.zoom_in();
}

void CyclesShaderEditor::EditGraphView::zoom_out()
{
	zoom.zoom_out();
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
			selection->nodes.clear();
			selection->nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
			break;
		case SelectMode::ADD:
			selection->nodes.insert(boxed_nodes.begin(), boxed_nodes.end());
			break;
		case SelectMode::TOGGLE:
			for (EditorNode* this_node : boxed_nodes) {
				if (selection->nodes.count(this_node) == 1) {
					selection->nodes.erase(this_node);
				}
				else {
					selection->nodes.insert(this_node);
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
	if (selection->socket == label) {
		return;
	}
	if (selection->socket != nullptr) {
		selection->socket->selected = false;
	}
	if (label != nullptr) {
		label->selected = true;
	}
	selection->socket = label;
}

void CyclesShaderEditor::EditGraphView::raise_node(EditorNode* node, SelectMode mode)
{
	if (node == nullptr) {
		return;
	}

	for (EditorNode* this_node : graph->nodes) {
		if (this_node == node) {
			graph->nodes.remove(this_node);
			graph->nodes.push_front(this_node);

			if (mode == SelectMode::EXCLUSIVE) {
				selection->nodes.clear();
				selection->nodes.insert(this_node);
			}
			else if (mode == SelectMode::ADD) {
				selection->nodes.insert(this_node);
			}
			else if (mode == SelectMode::TOGGLE) {
				if (selection->nodes.count(this_node) == 1) {
					selection->nodes.erase(this_node);
				}
				else {
					selection->nodes.insert(this_node);
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

	if (world_box_select_begin.get_x() < world_box_select_end.get_x()) {
		min_x_pos = world_box_select_begin.get_x();
		max_x_pos = world_box_select_end.get_x();
	}
	else {
		min_x_pos = world_box_select_end.get_x();
		max_x_pos = world_box_select_begin.get_x();
	}
	assert(min_x_pos <= max_x_pos);

	if (world_box_select_begin.get_y() < world_box_select_end.get_y()) {
		min_y_pos = world_box_select_begin.get_y();
		max_y_pos = world_box_select_end.get_y();
	}
	else {
		min_y_pos = world_box_select_end.get_y();
		max_y_pos = world_box_select_begin.get_y();
	}
	assert(min_y_pos <= max_y_pos);

	CyclesShaderEditor::FloatPos min_position(min_x_pos, min_y_pos);
	CyclesShaderEditor::FloatPos max_position(max_x_pos, max_y_pos);

	for (EditorNode* this_node : graph->nodes) {
		if (do_rectangles_overlap(min_position, max_position, this_node->world_pos, this_node->world_pos + this_node->get_dimensions())) {
			result.insert(this_node);
		}
	}

	return result;
}
