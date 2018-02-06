#pragma once

#include <string>
#include <vector>

#include "click_target.h"
#include "output.h"
#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	struct OutputNode;

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

		virtual std::string get_title();

		virtual void draw_node(NVGcontext* draw_context);
		virtual void set_mouse_position(Point2 node_local_position);

		virtual bool is_mouse_over_node();
		virtual bool is_mouse_over_header();
		virtual void handle_mouse_button(int button, int action, int mods);

		virtual void move_begin();
		virtual void move_end();

		virtual NodeSocket* get_socket_under_mouse();
		virtual NodeSocket* get_socket_label_under_mouse();
		virtual NodeSocket* get_socket_by_display_name(SocketInOut in_out, std::string socket_name);
		virtual NodeSocket* get_socket_by_internal_name(SocketInOut in_out, std::string socket_name);

		virtual Point2 get_dimensions();

		virtual bool can_be_deleted();

		virtual void update_output_node(OutputNode& output);

		bool selected = false;
		bool changed = true;

		CyclesNodeType type = CyclesNodeType::Unknown;

		Point2 world_pos;

	protected:
		std::string title;

		Point2 mouse_local_pos;
		Point2 mouse_local_begin_move_pos;

		bool node_moving = false;
		bool has_moved;

		float content_width = 146.0f;
		float content_height = 0.0f;

		std::vector<NodeSocket*> sockets;
		std::vector<SocketClickTarget> socket_targets;
		std::vector<SocketClickTarget> label_targets;
	};

}
