#include "editable_graph.h"

#include "float_pos.h"
#include "node_outputs.h"
#include "sockets.h"

cse::EditableGraph::EditableGraph(const ShaderGraphType type)
{
	reset(type);
}

void cse::EditableGraph::add_node(std::shared_ptr<EditableNode>& node, const FloatPos world_pos)
{
	if (node.get() == nullptr) {
		return;
	}

	node->world_pos = world_pos;
	nodes.push_front(node);
	should_push_undo_state = true;
}

void cse::EditableGraph::add_connection(const std::weak_ptr<NodeSocket> socket_begin, const std::weak_ptr<NodeSocket> socket_end)
{
	EditableNode* source_node = nullptr;
	if (const auto socket_begin_ptr = socket_begin.lock()) {
		if (socket_begin_ptr->io_type != SocketIOType::OUTPUT) {
			return;
		}
		source_node = socket_begin_ptr->parent;
	}
	else {
		// pointer is invalid
		return;
	}
	if (const auto socket_end_ptr = socket_end.lock()) {
		if (socket_end_ptr->io_type != SocketIOType::INPUT) {
			return;
		}
		if (socket_end_ptr->parent == source_node) {
			// Do not allow a node to connect to itself
			return;
		}
	}
	else {
		// pointer is invalid
		return;
	}
	// Remove any existing connection with this endpoint
	remove_connection_with_end(socket_end);
	should_push_undo_state = true;
	NodeConnection new_connection(socket_begin, socket_end);
	connections.push_back(new_connection);
}

cse::NodeConnection cse::EditableGraph::remove_connection_with_end(const std::weak_ptr<NodeSocket> socket_end)
{
	const NodeConnection default_result = NodeConnection(std::weak_ptr<NodeSocket>(), std::weak_ptr<NodeSocket>());
	if (socket_end.expired()) {
		return default_result;
	}
	std::list<NodeConnection>::iterator iter;
	for (iter = connections.begin(); iter != connections.end(); iter++) {
		if (iter->end_socket.lock() == socket_end.lock()) {
			should_push_undo_state = true;
			NodeConnection result = *iter;
			connections.erase(iter);
			return result;
		}
	}
	return default_result;
}

void cse::EditableGraph::remove_node_set(const cse::WeakNodeSet& weak_nodes_to_remove)
{
	// Create a set of shared_ptrs from the weak_ptrs
	SharedNodeSet shared_nodes_to_remove;
	for (auto weak_node : weak_nodes_to_remove) {
		shared_nodes_to_remove.insert(weak_node.lock());
	}

	// Remove all nodes in the set
	auto node_iter = nodes.begin();
	while (node_iter != nodes.end()) {
		auto this_node = *node_iter;
		if (this_node->can_be_deleted() && shared_nodes_to_remove.count(this_node) == 1) {
			node_iter = nodes.erase(node_iter);
			should_push_undo_state = true;
		}
		else {
			node_iter++;
		}
	}

	remove_invalid_connections();
}

bool cse::EditableGraph::is_node_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		if (this_node->is_under_point(world_pos)) {
			return true;
		}
	}
	return false;
}

std::weak_ptr<cse::EditableNode> cse::EditableGraph::get_node_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		if (this_node->is_under_point(world_pos)) {
			return this_node;
		}
	}
	return std::weak_ptr<EditableNode>();
}

std::weak_ptr<cse::NodeSocket> cse::EditableGraph::get_socket_under_point(const FloatPos world_pos) const
{
	for (const auto this_node : nodes) {
		auto maybe_result = this_node->get_socket_label_under_point(world_pos);
		if (maybe_result.lock()) {
			return maybe_result;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<cse::NodeSocket> cse::EditableGraph::get_connector_under_point(const FloatPos world_pos, const SocketIOType io_type) const
{
	for (const auto this_node : nodes) {
		auto maybe_result = this_node->get_socket_connector_under_point(world_pos);
		if (auto maybe_result_ptr = maybe_result.lock()) {
			if (maybe_result_ptr->io_type == io_type) {
				return maybe_result;
			}
		}
	}
	return std::weak_ptr<NodeSocket>();
}

void cse::EditableGraph::raise_node(const std::weak_ptr<cse::EditableNode> weak_node)
{
	const std::shared_ptr<EditableNode> node_to_raise = weak_node.lock();

	if (node_to_raise.get() == nullptr) {
		return;
	}

	// Exit early if this is already the top node
	if (nodes.front() == node_to_raise) {
		return;
	}

	for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
		const std::shared_ptr<EditableNode> this_node = *iter;
		if (this_node == node_to_raise) {
			nodes.erase(iter);
			nodes.push_front(this_node);
			return;
		}
	}
}

bool cse::EditableGraph::needs_undo_push()
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

void cse::EditableGraph::reset(const ShaderGraphType type)
{
	connections.clear();
	nodes.clear();

	switch (type) {
		case ShaderGraphType::EMPTY:
			// Do nothing
			break;
		case ShaderGraphType::MATERIAL:
		{
			nodes.push_back(std::make_shared<MaterialOutputNode>(FloatPos(0.0f, 0.0f)));
		}
	}
}

void cse::EditableGraph::remove_invalid_connections()
{
	std::list<NodeConnection>::iterator iter = connections.begin();
	while (iter != connections.end()) {
		if (iter->is_valid()) {
			iter++;
		}
		else {
			iter = connections.erase(iter);
		}
	}
}
