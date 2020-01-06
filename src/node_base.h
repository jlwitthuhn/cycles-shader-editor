#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "util_area.h"
#include "util_enum.h"

struct NVGcontext;

namespace cse {

	class NodeSocket;
	class EditableNode;

	class NodeConnection {
	public:
		NodeConnection(std::weak_ptr<NodeSocket> begin_socket, std::weak_ptr<NodeSocket> end_socket);

		bool is_valid() const;
		bool includes_node(EditableNode* node) const;

		const std::weak_ptr<NodeSocket> begin_socket;
		const std::weak_ptr<NodeSocket> end_socket;
	};

	class EditableNode {
	public:
		EditableNode(NodeCategory category, CyclesNodeType type, const std::string& title);
		virtual ~EditableNode() {}

		virtual std::string get_title() const;

		virtual void draw_node(NVGcontext* draw_context, bool selected, std::shared_ptr<NodeSocket> selected_socket);

		virtual bool contains_point(FloatPos world_pos) const;
		virtual std::weak_ptr<NodeSocket> get_socket_connector_under_point(FloatPos check_world_pos) const;
		virtual std::weak_ptr<NodeSocket> get_socket_label_under_point(FloatPos check_world_pos) const;

		virtual std::weak_ptr<NodeSocket> get_socket_by_display_name(SocketIOType in_out, const std::string& socket_name);
		virtual std::weak_ptr<NodeSocket> get_socket_by_internal_name(SocketIOType in_out, const std::string& socket_name);

		virtual FloatPos get_dimensions();

		virtual bool can_be_deleted();

		virtual void update_output_node(OutputNode& output);

		bool changed = true;

		FloatPos world_pos;

	protected:
		FloatPos get_local_pos(FloatPos world_pos_in) const;

		const NodeCategory category;
		const CyclesNodeType type;

		const std::string title;

		float content_width = 150.0f;
		float content_height = 0.0f;

		std::vector<std::shared_ptr<NodeSocket>> sockets;
		std::vector<HolderArea<std::weak_ptr<NodeSocket>>> socket_targets;
		std::vector<HolderArea<std::weak_ptr<NodeSocket>>> label_targets;
	};

}
