#include "drawing.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"
#include "util_area.h"
#include "util_enum.h"
#include "util_vector.h"
#include "wrapper_nvg_func.h"

void cse::Drawing::draw_button(
	NVGcontext* const draw_context,
	const Float2 pos,
	const float width,
	const float height,
	const std::string& label,
	const bool enabled,
	const bool pressed
	)
{
	const unsigned char gradient_alpha = enabled ? 41 : 20;
	const unsigned char button_alpha = enabled ? 170 : 95;
	const unsigned char label_alpha = enabled ? 255 : 100;

	const float x_pos = pos.x;
	const float y_pos = pos.y;

	const unsigned char start_color = pressed ? 0 : 255;
	const unsigned char end_color = pressed ? 127 : 15;

	NVGpaint button_bg = nvgLinearGradient(
		draw_context,
		x_pos,
		y_pos,
		x_pos,
		y_pos + height,
		nvgRGBA(start_color, start_color, start_color, gradient_alpha),
		nvgRGBA(end_color, end_color, end_color, gradient_alpha)
	);

	// Draw button
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context,
		x_pos,
		y_pos,
		width,
		height,
		UI_BUTTON_CORNER_RADIUS);
	nvgFillPaint(draw_context, button_bg);
	nvgFill(draw_context);
	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, button_alpha));
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStroke(draw_context);

	// Button label
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, label_alpha));
	nvgText(draw_context, x_pos + width / 2, y_pos + height / 2, label.c_str(), nullptr);
}

void cse::Drawing::draw_color_swatch(NVGcontext* const draw_context, const Float2 pos, const float width, const float height, const Float3 color, const bool selected)
{
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, pos.x, pos.y, width, height, UI_BUTTON_CORNER_RADIUS);
	nvgFillColor(draw_context, color);
	nvgFill(draw_context);
	if (selected) {
		nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 255));
	}
	else {
		nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
	}
	nvgStrokeWidth(draw_context, 1.5f);
	nvgStroke(draw_context);
}

void cse::Drawing::draw_color_pick_cursor(NVGcontext* const draw_context, const Float2 pos)
{
	constexpr float CURSOR_LENGTH_SHORT = 3.0f;
	constexpr float CURSOR_LENGTH_LONG = 8.0f;

	const float x_pos = pos.x;
	const float y_pos = pos.y;

	nvgBeginPath(draw_context);

	const float left_x = x_pos - CURSOR_LENGTH_SHORT - CURSOR_LENGTH_LONG;
	const float left_w = CURSOR_LENGTH_LONG;
	const float left_y = y_pos - CURSOR_LENGTH_SHORT / 2;
	const float left_h = CURSOR_LENGTH_SHORT;
	nvgRect(draw_context, left_x, left_y, left_w, left_h);

	const float right_x = x_pos + CURSOR_LENGTH_SHORT;
	const float right_w = CURSOR_LENGTH_LONG;
	const float right_y = y_pos - CURSOR_LENGTH_SHORT / 2;
	const float right_h = CURSOR_LENGTH_SHORT;
	nvgRect(draw_context, right_x, right_y, right_w, right_h);

	const float top_x = x_pos - CURSOR_LENGTH_SHORT / 2;
	const float top_w = CURSOR_LENGTH_SHORT;
	const float top_y = y_pos - CURSOR_LENGTH_SHORT - CURSOR_LENGTH_LONG;
	const float top_h = CURSOR_LENGTH_LONG;
	nvgRect(draw_context, top_x, top_y, top_w, top_h);

	const float bot_x = x_pos - CURSOR_LENGTH_SHORT / 2;
	const float bot_w = CURSOR_LENGTH_SHORT;
	const float bot_y = y_pos + CURSOR_LENGTH_SHORT;
	const float bot_h = CURSOR_LENGTH_LONG;
	nvgRect(draw_context, bot_x, bot_y, bot_w, bot_h);

	nvgFillColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
	nvgFill(draw_context);
	nvgStrokeColor(draw_context, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStroke(draw_context);
}

float cse::Drawing::draw_node(
	NVGcontext* const draw_context,
	const char* const title,
	const Float3 header_color,
	const float node_width,
	const Float2 world_pos,
	const bool selected,
	const std::vector<std::shared_ptr<NodeSocket>>& socket_vec,
	const std::shared_ptr<cse::NodeSocket>& selected_socket,
	std::vector<HolderArea<std::weak_ptr<NodeSocket>>>& socket_targets,
	std::vector<HolderArea<std::weak_ptr<NodeSocket>>>& label_targets
	)
{
	socket_targets.clear();
	label_targets.clear();

	const float draw_pos_x = 0.0f;
	float draw_pos_y = 0.0f;

	const float node_height = socket_vec.size() * UI_NODE_SOCKET_ROW_HEIGHT + UI_NODE_BOTTOM_PADDING;

	// Draw window
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, node_width, node_height + UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Draw header
	{
		const float rect_pos_y = draw_pos_y + UI_NODE_HEADER_HEIGHT - UI_NODE_CORNER_RADIUS;
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, node_width, UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
		nvgRect(draw_context, draw_pos_x, rect_pos_y, node_width, UI_NODE_CORNER_RADIUS);
		nvgFillColor(draw_context, header_color);
		nvgFill(draw_context);
	}

	nvgBeginPath(draw_context);
	nvgMoveTo(draw_context, draw_pos_x, UI_NODE_HEADER_HEIGHT);
	nvgLineTo(draw_context, draw_pos_x + node_width, UI_NODE_HEADER_HEIGHT);
	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 225));
	nvgStrokeWidth(draw_context, 1.3f);
	nvgStroke(draw_context);

	// Draw border
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, draw_pos_x, draw_pos_y, node_width, node_height + UI_NODE_HEADER_HEIGHT, UI_NODE_CORNER_RADIUS);
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
	nvgText(draw_context, draw_pos_x + node_width / 2, draw_pos_y + UI_NODE_HEADER_HEIGHT / 2, title, nullptr);

	float next_draw_y = draw_pos_y + UI_NODE_HEADER_HEIGHT + 2.0f;
	// Sockets
	for (const auto this_socket : socket_vec) {
		// Generate the text that will be used on this socket's label
		std::string label_text;
		std::string text_before_crossout; // For measuring text size later
		if (this_socket->value.use_count() > 0) {
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
		if (this_socket == selected_socket) {
			nvgBeginPath(draw_context);
			nvgRoundedRect(draw_context, 4.0f, next_draw_y, node_width - 8.0f, UI_NODE_SOCKET_ROW_HEIGHT, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
			nvgFill(draw_context);
		}

		// Draw label
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
		nvgFontFace(draw_context, "sans");

		if (this_socket->value.use_count() > 0 && this_socket->socket_type == SocketType::COLOR) {
			const float SWATCH_HEIGHT = 14.0f;
			const float SWATCH_WIDTH = 24.0f;
			const float SWATCH_CORNER_RADIUS = 8.0f;

			float bounds[4];
			nvgTextBounds(draw_context, 0.0f, 0.0f, label_text.c_str(), nullptr, bounds);
			const float label_width = bounds[2] - bounds[0];

			const float text_pos_x = draw_pos_x + node_width / 2 - SWATCH_WIDTH / 2;
			const float text_pos_y = next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2;
			nvgText(draw_context, text_pos_x, text_pos_y, label_text.c_str(), nullptr);

			const Float3 swatch_color = dynamic_cast<ColorSocketValue*>(this_socket->value.get())->get_value();

			const float swatch_pos_x = text_pos_x + label_width / 2 + 1.0f;
			const float swatch_pos_y = next_draw_y + (UI_NODE_SOCKET_ROW_HEIGHT - SWATCH_HEIGHT) / 2;

			nvgBeginPath(draw_context);
			nvgRoundedRect(draw_context, swatch_pos_x, swatch_pos_y, SWATCH_WIDTH, SWATCH_HEIGHT, SWATCH_CORNER_RADIUS);
			nvgFillColor(draw_context, swatch_color);
			nvgFill(draw_context);
			nvgStrokeWidth(draw_context, 1.0f);
			nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
			nvgStroke(draw_context);

			if (this_socket->input_connected_this_frame && this_socket->value.use_count() > 0) {
				const float x1 = swatch_pos_x - 3.0f;
				const float x2 = swatch_pos_x + SWATCH_WIDTH + 3.0f;
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
			const float text_pos_x = draw_pos_x + node_width / 2;
			const float text_pos_y = next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2;
			nvgText(draw_context, text_pos_x, text_pos_y, label_text.c_str(), nullptr);
			if (this_socket->input_connected_this_frame && this_socket->value.use_count() > 0) {
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
			const Float2 click_target_begin(0, next_draw_y - draw_pos_y);
			const Float2 click_target_end(node_width, next_draw_y - draw_pos_y + UI_NODE_SOCKET_ROW_HEIGHT);
			const HolderArea<std::weak_ptr<NodeSocket>> label_target(click_target_begin, click_target_end, this_socket);
			label_targets.push_back(label_target);
		}

		if (this_socket->draw_socket) {
			Float2 socket_position;
			if (this_socket->io_type == SocketIOType::INPUT) {
				socket_position = Float2(draw_pos_x, next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2);
			}
			else {
				socket_position = Float2(draw_pos_x + node_width, next_draw_y + UI_NODE_SOCKET_ROW_HEIGHT / 2);
			}
			nvgBeginPath(draw_context);
			nvgCircle(draw_context, socket_position.x, socket_position.y, UI_NODE_SOCKET_RADIUS);

			this_socket->world_draw_position = world_pos + socket_position;

			// Add click target for this socket
			Float2 local_socket_position(socket_position.x - draw_pos_x, socket_position.y - draw_pos_y);
			Float2 click_target_begin(local_socket_position.x - 7.0f, local_socket_position.y - 7.0f);
			Float2 click_target_end(local_socket_position.x + 7.0f, local_socket_position.y + 7.0f);
			const HolderArea<std::weak_ptr<NodeSocket>> socket_target(click_target_begin, click_target_end, this_socket);
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

	return node_height;
}

void cse::Drawing::draw_node_connection_curve(
	NVGcontext* const draw_context,
	const Float2 begin_pos,
	const Float2 end_pos,
	const float width
	)
{
	const float source_x = begin_pos.x;
	const float source_y = begin_pos.y;
	const float dest_x = end_pos.x;
	const float dest_y = end_pos.y;

	const float c_distance = fabs(dest_x - source_x) / 2.0f;
	const float c1_x = source_x + c_distance;
	const float c2_x = dest_x - c_distance;

	nvgBeginPath(draw_context);
	nvgMoveTo(draw_context, source_x, source_y);
	nvgBezierTo(draw_context, c1_x, source_y, c2_x, dest_y, dest_x, dest_y);
	nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 255));
	nvgStrokeWidth(draw_context, width);
	nvgStroke(draw_context);
}