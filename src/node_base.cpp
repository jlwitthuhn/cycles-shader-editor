#include "node_base.h"

#include <iomanip>
#include <map>
#include <sstream>

#include <nanovg.h>

#include "config.h"
#include "curve.h"
#include "gui_sizes.h"
#include "sockets.h"

CyclesShaderEditor::NodeConnection::NodeConnection(const std::weak_ptr<NodeSocket> begin_socket, const std::weak_ptr<NodeSocket> end_socket) :
	begin_socket(begin_socket),
	end_socket(end_socket)
{

}

bool CyclesShaderEditor::NodeConnection::is_valid() const
{
	return !(begin_socket.expired() || end_socket.expired());
}

bool CyclesShaderEditor::NodeConnection::includes_node(EditorNode* const node) const
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

std::string CyclesShaderEditor::EditorNode::get_title() const
{
	return title;
}

void CyclesShaderEditor::EditorNode::draw_node(NVGcontext* const draw_context, const bool selected)
{
	float draw_pos_x = 0.0f;
	float draw_pos_y = 0.0f;

	content_height = sockets.size() * UI_NODE_SOCKET_ROW_HEIGHT + UI_NODE_BOTTOM_PADDING;

	// Draw window
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, content_width, content_height + UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Draw header
	{
		const float rect_pos_y = draw_pos_y + UI_NODE_HEADER_HEIGHT - UI_NODE_CORNER_RADIUS;
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, content_width, UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
		nvgRect(draw_context, draw_pos_x, rect_pos_y, content_width, UI_NODE_CORNER_RADIUS);
		if (highlight_header) {
			nvgFillColor(draw_context, nvgRGBA(225, 225, 225, 255));
		}
		else {
			nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
		}
		nvgFill(draw_context);
	}

	// Draw border
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, content_width, content_height + UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
	if (selected) {
		nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 225));
	}
	else {
		nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 225));
	}
	nvgStrokeWidth(draw_context, 1.5f);
	nvgStroke(draw_context);

	// Title
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, draw_pos_x + content_width / 2, draw_pos_y + UI_NODE_HEADER_HEIGHT / 2, title.c_str(), nullptr);

	float next_draw_y = draw_pos_y + UI_NODE_HEADER_HEIGHT + 2.0f;
	// Sockets
	label_targets.clear();
	socket_targets.clear();
	for (const auto this_socket: sockets) {
		// Generate the text that will be used on this socket's label
		std::string label_text;
		std::string text_before_crossout; // For measuring text size later
		if (this_socket->value != nullptr) {
			text_before_crossout = this_socket->display_name + ":";
			if (this_socket->socket_type == SocketType::FLOAT) {
				const std::shared_ptr<FloatSocketValue> float_val = std::dynamic_pointer_cast<FloatSocketValue>(this_socket->value);
				if (float_val) {
					std::stringstream label_string_stream;
					label_string_stream << this_socket->display_name << ": " << std::fixed << std::setprecision(3) << float_val->get_value();
					label_text = label_string_stream.str();
				}
			}
			else if (this_socket->socket_type == SocketType::VECTOR) {
				if (this_socket->selectable == true) {
					label_text = this_socket->display_name + ": [Vector]";
				}
				else {
					label_text = this_socket->display_name;
				}
			}
			else if (this_socket->socket_type == SocketType::COLOR) {
				std::stringstream label_string_stream;
				label_string_stream << this_socket->display_name << ": ";
				label_text = label_string_stream.str();
			}
			else if (this_socket->socket_type == SocketType::STRING_ENUM) {
				label_text = this_socket->display_name + ": [Enum]";
			}
			else if (this_socket->socket_type == SocketType::INT) {
				const std::shared_ptr<IntSocketValue> int_val = std::dynamic_pointer_cast<IntSocketValue>(this_socket->value);
				if (int_val) {
					std::stringstream label_string_stream;
					label_string_stream << this_socket->display_name << ": " << std::fixed << std::setprecision(3) << int_val->get_value();
					label_text = label_string_stream.str();
				}
			}

			else if (this_socket->socket_type == SocketType::BOOLEAN) {
				const std::shared_ptr<BoolSocketValue> bool_val = std::dynamic_pointer_cast<BoolSocketValue>(this_socket->value);
				if (bool_val) {
					if (bool_val->value) {
						label_text = this_socket->display_name + ": True";
					}
					else {
						label_text = this_socket->display_name + ": False";
					}
				}
			}
			else if (this_socket->socket_type == SocketType::CURVE) {
				label_text = this_socket->display_name + ": [Curve]";
			}
			else {
				label_text = this_socket->display_name;
			}
		}
		else {
			label_text = this_socket->display_name;
		}

		// Draw highlight if this node is selected
		if (this_socket->selected) {
			nvgBeginPath(draw_context);
			nvgRoundedRect(draw_context, 4.0f, next_draw_y, content_width - 8.0f, UI_NODE_SOCKET_ROW_HEIGHT, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
			nvgFill(draw_context);
		}

		// Draw label
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
		nvgFontFace(draw_context, "sans");

		if (this_socket->value != nullptr && this_socket->socket_type == SocketType::COLOR) {
			const float SWATCH_HEIGHT = 14.0f;
			const float SWATCH_WIDTH = 24.0f;
			const float SWATCH_CORNER_RADIUS = 8.0f;

			float bounds[4];
			nvgTextBounds(draw_context, 0.0f, 0.0f, label_text.c_str(), nullptr, bounds);
			const float label_width = bounds[2] - bounds[0];

			const float text_pos_x = draw_pos_x + content_width / 2 - SWATCH_WIDTH / 2;
			const float text_pos_y = next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2;
			nvgText(draw_context, text_pos_x, text_pos_y, label_text.c_str(), nullptr);

			const FloatRGBColor swatch_color = dynamic_cast<ColorSocketValue*>(this_socket->value.get())->get_value();

			const float swatch_pos_x = text_pos_x + label_width / 2 + 1.0f;
			const float swatch_pos_y = next_draw_y + (UI_NODE_SOCKET_ROW_HEIGHT - SWATCH_HEIGHT) / 2;

			nvgBeginPath(draw_context);
			nvgRoundedRect(draw_context, swatch_pos_x, swatch_pos_y, SWATCH_WIDTH, SWATCH_HEIGHT, SWATCH_CORNER_RADIUS);
			nvgFillColor(draw_context, nvgRGBAf(swatch_color.r, swatch_color.g, swatch_color.b, 1.0f));
			nvgFill(draw_context);
			nvgStrokeWidth(draw_context, 1.0f);
			nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
			nvgStroke(draw_context);

			if (this_socket->input_connected_this_frame && this_socket->value != nullptr) {
				const float x1 = swatch_pos_x - 3.0f;
				const float x2 = swatch_pos_x  + SWATCH_WIDTH + 3.0f;
				const float y = swatch_pos_y + SWATCH_HEIGHT / 2.0f;
				nvgBeginPath(draw_context);
				nvgMoveTo(draw_context, x1, y);
				nvgLineTo(draw_context, x2, y);
				nvgStrokeWidth(draw_context, 1.2f);
				nvgStroke(draw_context);
				this_socket->input_connected_this_frame = false;
			}
		}
		else {
			const float text_pos_x = draw_pos_x + content_width / 2;
			const float text_pos_y = next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2;
			nvgText(draw_context, text_pos_x, text_pos_y, label_text.c_str(), nullptr);
			if (this_socket->input_connected_this_frame && this_socket->value != nullptr) {
				// Output is [xmin, ymin, xmax, ymax]
				float full_size[4];
				float short_size[4];
				nvgTextBounds(draw_context, text_pos_x, text_pos_y, label_text.c_str(), nullptr, full_size);
				nvgTextBounds(draw_context, text_pos_x, text_pos_y, text_before_crossout.c_str(), nullptr, short_size);
				const float short_width = short_size[2] - short_size[0];
				const float x1 = full_size[0] + short_width + 1.0f;
				const float x2 = full_size[2] + 2.0f;
				nvgBeginPath(draw_context);
				nvgMoveTo(draw_context, x1, text_pos_y);
				nvgLineTo(draw_context, x2, text_pos_y);
				nvgStrokeWidth(draw_context, 1.2f);
				nvgStroke(draw_context);
				this_socket->input_connected_this_frame = false;
			}
		}

		if (this_socket->selectable) {
			// Add label click target
			const CyclesShaderEditor::FloatPos click_target_begin(0, next_draw_y - draw_pos_y);
			const CyclesShaderEditor::FloatPos click_target_end(content_width, next_draw_y - draw_pos_y + UI_NODE_SOCKET_ROW_HEIGHT);
			const SocketArea label_target(click_target_begin, click_target_end, this_socket);
			label_targets.push_back(label_target);
		}

		if (this_socket->draw_socket) {
			CyclesShaderEditor::FloatPos socket_position;
			if (this_socket->io_type == SocketIOType::INPUT) {
				socket_position = CyclesShaderEditor::FloatPos(draw_pos_x, next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2);
			}
			else {
				socket_position = CyclesShaderEditor::FloatPos(draw_pos_x + content_width, next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2);
			}
			nvgBeginPath(draw_context);
			nvgCircle(draw_context, socket_position.get_x(), socket_position.get_y(), UI_NODE_SOCKET_RADIUS);

			this_socket->world_draw_position = world_pos + socket_position;

			// Add click target for this socket
			CyclesShaderEditor::FloatPos local_socket_position(socket_position.get_x() - draw_pos_x, socket_position.get_y() - draw_pos_y);
			CyclesShaderEditor::FloatPos click_target_begin(local_socket_position.get_x() - 7.0f, local_socket_position.get_y() - 7.0f);
			CyclesShaderEditor::FloatPos click_target_end(local_socket_position.get_x() + 7.0f, local_socket_position.get_y() + 7.0f);
			SocketArea socket_target(click_target_begin, click_target_end, this_socket);
			socket_targets.push_back(socket_target);

			if (this_socket->socket_type == SocketType::CLOSURE) {
				nvgFillColor(draw_context, nvgRGBA(100, 200, 100, 255));
			}
			else if (this_socket->socket_type == SocketType::COLOR) {
				nvgFillColor(draw_context, nvgRGBA(200, 200, 42, 255));
			}
			else if (this_socket->socket_type == SocketType::FLOAT) {
				nvgFillColor(draw_context, nvgRGBA(240, 240, 240, 255));
			}
			else if (this_socket->socket_type == SocketType::NORMAL) {
				nvgFillColor(draw_context, nvgRGBA(100, 100, 200, 255));
			}
			else if (this_socket->socket_type == SocketType::VECTOR) {
				nvgFillColor(draw_context, nvgRGBA(100, 100, 200, 255));
			}
			else {
				nvgFillColor(draw_context, nvgRGBA(255, 0, 0, 255));
			}

			nvgFill(draw_context);

			nvgStrokeColor(draw_context, nvgRGBf(0.0f, 0.0f, 0.0f));
			nvgStrokeWidth(draw_context, 1.0f);
			nvgStroke(draw_context);
		}

		next_draw_y += UI_NODE_SOCKET_ROW_HEIGHT;
	}
}

bool CyclesShaderEditor::EditorNode::is_under_point(const FloatPos check_world_pos) const
{
	const FloatPos local_pos = get_local_pos(check_world_pos);
	return (
		local_pos.get_x() >= 0.0f &&
		local_pos.get_x() <= content_width &&
		local_pos.get_y() >= 0.0f &&
		local_pos.get_y() <= UI_NODE_HEADER_HEIGHT + content_height
	);
}

std::weak_ptr<CyclesShaderEditor::NodeSocket> CyclesShaderEditor::EditorNode::get_socket_connector_under_point(const FloatPos check_world_pos) const
{
	const FloatPos local_pos = get_local_pos(check_world_pos);
	for (const auto click_target : socket_targets) {
		if (click_target.is_under_point(local_pos)) {
			return click_target.socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<CyclesShaderEditor::NodeSocket> CyclesShaderEditor::EditorNode::get_socket_label_under_point(const FloatPos check_world_pos) const
{
	if (is_under_point(check_world_pos) == false) {
		// Nothing will match if the node is not under the given point
		return std::weak_ptr<NodeSocket>();
	}
	const FloatPos local_pos = get_local_pos(check_world_pos);
	for (const auto click_target : label_targets) {
		if (click_target.is_under_point(local_pos)) {
			return click_target.socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<CyclesShaderEditor::NodeSocket> CyclesShaderEditor::EditorNode::get_socket_by_display_name(const SocketIOType in_out, const std::string& socket_name)
{
	for (const auto socket : sockets) {
		if (socket->display_name == socket_name && socket->io_type == in_out) {
			return socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

std::weak_ptr<CyclesShaderEditor::NodeSocket> CyclesShaderEditor::EditorNode::get_socket_by_internal_name(const SocketIOType in_out, const std::string& socket_name)
{
	for (const auto socket : sockets) {
		if (socket->internal_name == socket_name && socket->io_type == in_out) {
			return socket;
		}
	}
	return std::weak_ptr<NodeSocket>();
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::EditorNode::get_dimensions()
{
	return CyclesShaderEditor::FloatPos(content_width, content_height + UI_NODE_HEADER_HEIGHT);
}

bool CyclesShaderEditor::EditorNode::can_be_deleted()
{
	return true;
}

void CyclesShaderEditor::EditorNode::update_output_node(OutputNode& output)
{
	output.type = type;

	output.world_x = world_pos.get_floor_x();
	output.world_y = world_pos.get_floor_y();

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
				const float x = color_val->red_socket_val->get_value();
				const float y = color_val->green_socket_val->get_value();
				const float z = color_val->blue_socket_val->get_value();
				const Float3 float3_val(x, y, z);
				output.float3_values[this_socket->internal_name] = float3_val;
			}
		}
		else if (this_socket->socket_type == SocketType::VECTOR) {
			const std::shared_ptr<Float3SocketValue> float3_socket_val = std::dynamic_pointer_cast<Float3SocketValue>(this_socket->value);
			if (float3_socket_val) {
				const Float3Holder temp_value = float3_socket_val->get_value();
				const Float3 float3_val(temp_value.x, temp_value.y, temp_value.z);
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
				typedef std::vector<FloatPos>::size_type vec_index;
				for (vec_index i = 0; i < curve_val->curve_points.size(); i++) {
					const FloatPos this_point = curve_val->curve_points[i];
					out_curve.control_points.push_back(Float2(this_point.get_x(), this_point.get_y()));
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
	}
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::EditorNode::get_local_pos(FloatPos world_pos_in) const
{
	return world_pos_in - world_pos;
}
