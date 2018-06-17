#pragma once

#include <list>
#include <memory>
#include <set>

#include "common_enums.h"
#include "node_base.h"

namespace CyclesShaderEditor {
	
	class FloatPos;

	// This class is used to store all data for a single graph that can be edited interactively
	class EditableGraph {
	public:
		EditableGraph(ShaderGraphType type);

		void add_node(std::unique_ptr<EditorNode>& node, FloatPos world_pos);
		void add_connection(std::weak_ptr<NodeSocket> socket_begin, std::weak_ptr<NodeSocket> socket_end);
		
		NodeConnection remove_connection_with_end(std::weak_ptr<NodeSocket> socket_end);
		void remove_node_set(const std::set<EditorNode*>& nodes_to_remove);

		bool is_node_under_point(FloatPos world_pos) const;
		EditorNode* get_node_under_point(FloatPos world_pos) const;

		std::weak_ptr<NodeSocket> get_socket_under_point(FloatPos world_pos) const;
		std::weak_ptr<NodeSocket> get_connector_under_point(FloatPos world_pos, SocketIOType io_type) const;

		void raise_node(EditorNode* node);

		bool needs_undo_push();

		std::list<EditorNode*> nodes;
		std::list<NodeConnection> connections;

	private:
		void reset(ShaderGraphType type);

		void clear_nodes();
		void remove_invalid_connections();

		bool should_push_undo_state = false;
	};
}
