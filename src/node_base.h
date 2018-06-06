#pragma once

#include <string>
#include <vector>

#include "click_target.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class EditorNode;

	class NodeConnection {
	public:
		NodeConnection(NodeSocket* begin_socket, NodeSocket* end_socket);

		bool includes_node(EditorNode* node);

		NodeSocket* begin_socket = nullptr;
		NodeSocket* end_socket = nullptr;
	};

	class EditorNode {
	public:
		virtual ~EditorNode();

		virtual std::string get_title() const;

		virtual void draw_node(NVGcontext* draw_context);

		virtual bool is_node_under_point(FloatPos check_world_pos) const;
		virtual NodeSocket* get_socket_connector_under_point(FloatPos check_world_pos) const;
		virtual NodeSocket* get_socket_label_under_point(FloatPos check_world_pos) const;

		virtual NodeSocket* get_socket_by_display_name(SocketInOut in_out, const std::string& socket_name);
		virtual NodeSocket* get_socket_by_internal_name(SocketInOut in_out, const std::string& socket_name);

		virtual FloatPos get_dimensions();

		virtual bool can_be_deleted();

		virtual void update_output_node(OutputNode& output);

		bool selected = false;
		bool changed = true;

		CyclesNodeType type = CyclesNodeType::Unknown;

		FloatPos world_pos;

	protected:
		FloatPos get_local_pos(FloatPos world_pos_in) const;

		std::string title;

		bool highlight_header = false;

		float content_width = 150.0f;
		float content_height = 0.0f;

		std::vector<NodeSocket*> sockets;
		std::vector<SocketClickTarget> socket_targets;
		std::vector<SocketClickTarget> label_targets;
	};

}
