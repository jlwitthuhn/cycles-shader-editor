#include "editable_graph.h"

#include "float_pos.h"
#include "node_outputs.h"

CyclesShaderEditor::EditableGraph::EditableGraph(const ShaderGraphType type)
{
	reset(type);
}

void CyclesShaderEditor::EditableGraph::add_node(std::unique_ptr<EditorNode>& node, const FloatPos world_pos)
{
	if (node.get() == nullptr) {
		return;
	}

	EditorNode* const node_ptr = node.release();
	node_ptr->world_pos = world_pos;
	nodes.push_front(node_ptr);
	should_push_undo_state = true;
}

void CyclesShaderEditor::EditableGraph::add_connection(NodeSocket* const socket_begin, NodeSocket* const socket_end)
{
	if (socket_begin == nullptr || socket_end == nullptr) {
		return;
	}
	if (socket_begin->io_type != SocketIOType::Output || socket_end->io_type != SocketIOType::Input) {
		return;	
	}
	// Remove any existing connection with this endpoint
	remove_connection_with_end(socket_end);
	should_push_undo_state = true;
	NodeConnection new_connection(socket_begin, socket_end);
	connections.push_back(new_connection);
}

CyclesShaderEditor::NodeConnection CyclesShaderEditor::EditableGraph::remove_connection_with_end(NodeSocket* const socket_end)
{
	const NodeConnection default_result = NodeConnection(nullptr, nullptr);
	if (socket_end == nullptr) {
		return default_result;
	}
	std::list<NodeConnection>::iterator iter;
	for (iter = connections.begin(); iter != connections.end(); iter++) {
		if (iter->end_socket == socket_end) {
			should_push_undo_state = true;
			NodeConnection result = *iter;
			connections.erase(iter);
			return result;
		}
	}
	return default_result;
}

void CyclesShaderEditor::EditableGraph::remove_node_set(const std::set<EditorNode*>& nodes_to_remove)
{
	// Remove all nodes in the set
	std::list<EditorNode*>::iterator node_iter = nodes.begin();
	while (node_iter != nodes.end()) {
		EditorNode* const this_node = *node_iter;
		if (nodes_to_remove.count(this_node) == 1) {
			delete this_node;
			node_iter = nodes.erase(node_iter);
			should_push_undo_state = true;
		}
		else {
			node_iter++;
		}
	}

	// Remove all connections referencing these nodes
	std::list<NodeConnection>::iterator connection_iter = connections.begin();
	while (connection_iter != connections.end()) {
		const NodeConnection this_connection = *connection_iter;
		const bool begin_socket_matches = nodes_to_remove.count(this_connection.begin_socket->parent) == 1;
		const bool end_socket_matches = nodes_to_remove.count(this_connection.end_socket->parent) == 1;
		if (begin_socket_matches || end_socket_matches) {
			connection_iter = connections.erase(connection_iter);
			should_push_undo_state = true;
		}
		else {
			connection_iter++;
		}
	}
}

bool CyclesShaderEditor::EditableGraph::is_node_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		if (this_node->is_under_point(world_pos)) {
			return true;
		}
	}
	return false;
}

CyclesShaderEditor::EditorNode* CyclesShaderEditor::EditableGraph::get_node_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		if (this_node->is_under_point(world_pos)) {
			return this_node;
		}
	}
	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditableGraph::get_socket_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		auto maybe_result = this_node->get_socket_label_under_point(world_pos);
		if (maybe_result) {
			return maybe_result;
		}
	}
	return nullptr;
}

CyclesShaderEditor::NodeSocket* CyclesShaderEditor::EditableGraph::get_connector_under_point(const FloatPos world_pos, const SocketIOType io_type) const
{
	for (const auto this_node : nodes) {
		auto maybe_result = this_node->get_socket_connector_under_point(world_pos);
		if (maybe_result && maybe_result->io_type == io_type) {
			return maybe_result;
		}
	}
	return nullptr;
}

void CyclesShaderEditor::EditableGraph::raise_node(EditorNode* const node)
{
	// Exit early if this is already the top node
	if (nodes.front() == node) {
		return;
	}
	std::list<EditorNode*>::iterator iter;
	for (iter = nodes.begin(); iter != nodes.end(); iter++) {
		EditorNode* const this_node = *iter;
		if (this_node == node) {
			nodes.erase(iter);
			nodes.push_front(this_node);
			return;
		}
	}
}

bool CyclesShaderEditor::EditableGraph::needs_undo_push()
{
	bool result = false;
	for (const auto this_node : nodes) {
		if (this_node->changed) {
			result = true;
			this_node->changed = false;
		}
	}
	if (should_push_undo_state) {
		result = true;
		should_push_undo_state = false;
	}
	return result;
}

void CyclesShaderEditor::EditableGraph::reset(const ShaderGraphType type)
{
	connections.clear();
	clear_nodes();

	switch (type) {
	case ShaderGraphType::EMPTY:
		// Do nothing
		break;
	case ShaderGraphType::MATERIAL:
		{
			EditorNode* const output_node = new MaterialOutputNode(CyclesShaderEditor::FloatPos(0.0f, 0.0f));
			nodes.push_back(output_node);
		}
	}
}

// Remove this once nodes use smart pointers
void CyclesShaderEditor::EditableGraph::clear_nodes()
{
	for (const auto* this_node : nodes) {
		delete this_node;
	}
	nodes.clear();
}
