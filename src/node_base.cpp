#include "node_base.h"

#include <cmath>
#include <map>

#include "config.h"
#include "curve.h"
#include "drawing.h"
#include "gui_colors.h"
#include "gui_sizes.h"
#include "sockets.h"
#include "util_color_ramp.h"
#include "util_enum.h"
#include "util_vector.h"

static cse::Float3 get_color_for_category(const cse::NodeCategory category)
{
	using cse::NodeCategory;

	switch(category) {
		case NodeCategory::OUTPUT:
			return COLOR_NODE_HEADER_OUTPUT;
		case NodeCategory::COLOR:
			return COLOR_NODE_HEADER_COLOR;
		case NodeCategory::CONVERTER:
			return COLOR_NODE_HEADER_CONVERTER;
		case NodeCategory::INPUT:
			return COLOR_NODE_HEADER_INPUT;
		case NodeCategory::SHADER:
			return COLOR_NODE_HEADER_SHADER;
		case NodeCategory::TEXTURE:
			return COLOR_NODE_HEADER_TEXTURE;
		case NodeCategory::VECTOR:
			return COLOR_NODE_HEADER_VECTOR;
		default:
			return COLOR_NODE_HEADER_DEFAULT;
	}
}

cse::NodeConnection::NodeConnection(const std::weak_ptr<NodeSocket> begin_socket, const std::weak_ptr<NodeSocket> end_socket) :
	begin_socket(begin_socket),
	end_socket(end_socket)
{

}

bool cse::NodeConnection::is_valid() const
{
	return !(begin_socket.expired() || end_socket.expired());
}

bool cse::NodeConnection::includes_node(EditableNode* const node) const
{
	if (auto begin_socket_ptr = begin_socket.lock()) {
		if (begin_socket_ptr->parent == node) {
			return true;
		}
	}
	if (auto end_socket_ptr = end_socket.lock()) {
		if (end_socket_ptr->parent == node) {
			return true;
		}
	}
	return false;
}

cse::EditableNode::EditableNode(NodeCategory category, CyclesNodeType type, const std::string& title) :
	category(category),
	type(type),
	title(title)
{

}

std::string cse::EditableNode::get_title() const
{
	return title;
}

void cse::EditableNode::draw_node(NVGcontext* const draw_context, const bool selected, const std::shared_ptr<cse::NodeSocket> selected_socket)
{
	const Float3 header_color = get_color_for_category(category);

	content_height = Drawing::draw_node(
		draw_context,
		title.c_str(),
		header_color,
		content_width,
		world_pos,
		selected,
		sockets,
		selected_socket,
		socket_targets,
		label_targets
	);
}

bool cse::EditableNode::contains_point(const Float2 world_pos_in) const
{
	const Float2 local_pos = get_local_pos(world_pos_in);
	return (
		local_pos.x >= 0.0f &&
		local_pos.x <= content_width &&
		local_pos.y >= 0.0f &&
		local_pos.y <= UI_NODE_HEADER_HEIGHT + content_height
	);
}

std::weak_ptr<cse::NodeSocket> cse::EditableNode::get_socket_connector_under_point(const Float2 check_world_pos) const
{
	const Float2 local_pos = get_local_pos(check_world_pos);
	for (const auto click_target : socket_targets) {
		if (click_target.contains_point(local_pos)) {
			return click_target.get_value();
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<cse::NodeSocket> cse::EditableNode::get_socket_label_under_point(const Float2 check_world_pos) const
{
	if (contains_point(check_world_pos) == false) {
		// Nothing will match if the node is not under the given point
		return std::weak_ptr<NodeSocket>();
	}
	const Float2 local_pos = get_local_pos(check_world_pos);
	for (const auto click_target : label_targets) {
		if (click_target.contains_point(local_pos)) {
			return click_target.get_value();
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<cse::NodeSocket> cse::EditableNode::get_socket_by_display_name(const SocketIOType in_out, const std::string& socket_name)
{
	for (const auto socket : sockets) {
		if (socket->display_name == socket_name && socket->io_type == in_out) {
			return socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<cse::NodeSocket> cse::EditableNode::get_socket_by_internal_name(const SocketIOType in_out, const std::string& socket_name)
{
	for (const auto socket : sockets) {
		if (socket->internal_name == socket_name && socket->io_type == in_out) {
			return socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

cse::Float2 cse::EditableNode::get_dimensions()
{
	return cse::Float2(content_width, content_height + UI_NODE_HEADER_HEIGHT);
}

bool cse::EditableNode::can_be_deleted()
{
	return true;
}

void cse::EditableNode::update_output_node(OutputNode& output)
{
	output.type = type;

	output.world_x = std::floor(world_pos.x);
	output.world_y = std::floor(world_pos.y);

	if (type == CyclesNodeType::MaterialOutput) {
		output.name = std::string("output");
	}

	for (const auto this_socket : sockets) {
		if (this_socket->io_type != SocketIOType::INPUT) {
			continue;
		}

		if (this_socket->socket_type == SocketType::FLOAT) {
			const std::shared_ptr<FloatSocketValue> float_val = std::dynamic_pointer_cast<FloatSocketValue>(this_socket->value);
			if (float_val) {
				output.float_values[this_socket->internal_name] = float_val->get_value();
			}
		}
		else if (this_socket->socket_type == SocketType::COLOR) {
			const std::shared_ptr<ColorSocketValue> color_val = std::dynamic_pointer_cast<ColorSocketValue>(this_socket->value);
			if (color_val) {
				const float x = color_val->r_socket_val->get_value();
				const float y = color_val->g_socket_val->get_value();
				const float z = color_val->b_socket_val->get_value();
				const Float3 float3_val(x, y, z);
				output.float3_values[this_socket->internal_name] = float3_val;
			}
		}
		else if (this_socket->socket_type == SocketType::VECTOR) {
			const std::shared_ptr<Float3SocketValue> float3_socket_val = std::dynamic_pointer_cast<Float3SocketValue>(this_socket->value);
			if (float3_socket_val) {
				const Float3 float3_val = float3_socket_val->get_value();
				output.float3_values[this_socket->internal_name] = float3_val;
			}
		}
		else if (this_socket->socket_type == SocketType::STRING_ENUM) {
			const std::shared_ptr<StringEnumSocketValue> string_val = std::dynamic_pointer_cast<StringEnumSocketValue>(this_socket->value);
			if (string_val) {
				output.string_values[this_socket->internal_name] = string_val->value.internal_value;
			}
		}
		else if (this_socket->socket_type == SocketType::INT) {
			const std::shared_ptr<IntSocketValue> int_val = std::dynamic_pointer_cast<IntSocketValue>(this_socket->value);
			if (int_val) {
				output.int_values[this_socket->internal_name] = int_val->get_value();
			}
		}
		else if (this_socket->socket_type == SocketType::BOOLEAN) {
			const std::shared_ptr<BoolSocketValue> bool_val = std::dynamic_pointer_cast<BoolSocketValue>(this_socket->value);
			if (bool_val) {
				output.bool_values[this_socket->internal_name] = bool_val->value;
			}
		}
		else if (this_socket->socket_type == SocketType::CURVE) {
			const std::shared_ptr<CurveSocketValue> curve_val = std::dynamic_pointer_cast<CurveSocketValue>(this_socket->value);
			if (curve_val) {
				OutputCurve out_curve;
				typedef std::vector<Float2>::size_type vec_index;
				for (vec_index i = 0; i < curve_val->curve_points.size(); i++) {
					const Float2 this_point = curve_val->curve_points[i];
					out_curve.control_points.push_back(Float2(this_point.x, this_point.y));
				}
				out_curve.enum_curve_interp = static_cast<int>(curve_val->curve_interp);
				const CurveEvaluator curve(curve_val);
				for (vec_index i = 0; i < CURVE_TABLE_SIZE; i++) {
					const float x = static_cast<float>(i) / (CURVE_TABLE_SIZE - 1.0f);
					out_curve.samples.push_back(curve.eval(x));
				}
				output.curve_values[this_socket->internal_name] = out_curve;
			}
		}
		else if (this_socket->socket_type == SocketType::COLOR_RAMP) {
			const std::shared_ptr<ColorRampSocketValue> ramp_val = std::dynamic_pointer_cast<ColorRampSocketValue>(this_socket->value);
			if (ramp_val) {
				OutputColorRamp out_ramp;

				// Copy over control points
				for (const auto& this_point : ramp_val->ramp_points) {
					OutputColorRampPoint new_point;
					new_point.pos = this_point.position;
					new_point.color = this_point.color;
					new_point.alpha = this_point.alpha;
					out_ramp.points.push_back(new_point);
				}

				// Copy evaluated samples
				std::vector<Float4> samples = ramp_val->evaluate_samples();
				for (const auto& this_sample : samples) {
					out_ramp.samples_color.push_back(Float3(this_sample.x, this_sample.y, this_sample.z));
					out_ramp.samples_alpha.push_back(this_sample.w);
				}

				output.ramp_values[this_socket->internal_name] = out_ramp;
			}
		}
	}
}

cse::Float2 cse::EditableNode::get_local_pos(const Float2 world_pos_in) const
{
	return world_pos_in - world_pos;
}
