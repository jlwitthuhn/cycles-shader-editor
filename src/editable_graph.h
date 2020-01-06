#pragma once

#include <list>
#include <memory>
#include <set>

#include "common_enums.h"
#include "node_base.h"
#include "util_typedef.h"
#include "util_vector.h"

namespace cse {

	class NodeSocket;

	// This class is used to store all data for a single graph that can be edited interactively
	class EditableGraph {
	public:
		EditableGraph(ShaderGraphType type);

		void add_node(std::shared_ptr<EditableNode>& node, Float2 world_pos);
		void add_connection(std::weak_ptr<NodeSocket> socket_begin, std::weak_ptr<NodeSocket> socket_end);

		NodeConnection remove_connection_with_end(std::weak_ptr<NodeSocket> socket_end);
		void remove_node_set(const WeakNodeSet& nodes_to_remove);

		bool is_node_under_point(Float2 world_pos) const;
		std::weak_ptr<EditableNode> get_node_under_point(Float2 world_pos) const;

		std::weak_ptr<NodeSocket> get_socket_under_point(Float2 world_pos) const;
		std::weak_ptr<NodeSocket> get_connector_under_point(Float2 world_pos, SocketIOType io_type) const;

		void raise_node(std::weak_ptr<EditableNode> node);

		bool needs_undo_push();

		std::list<std::shared_ptr<EditableNode>> nodes;
		std::list<NodeConnection> connections;

	private:
		void reset(ShaderGraphType type);

		void remove_invalid_connections();

		bool should_push_undo_state = false;
	};
}
