#include "subwindow_param_editor.h"

#include <algorithm>
#include <cmath>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "input_box.h"
#include "node_base.h"

CyclesShaderEditor::ParamEditorSubwindow::ParamEditorSubwindow(Point2 screen_position) :
	NodeEditorSubwindow(screen_position),
	int_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	float_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_x_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_y_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_z_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_r_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_g_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_b_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT)
{
	subwindow_width = UI_SUBWIN_PARAM_EDIT_WIDTH;
	subwindow_height = UI_SUBWIN_HEADER_HEIGHT;
}

void CyclesShaderEditor::ParamEditorSubwindow::draw(NVGcontext* draw_context)
{
	if (is_subwindow_active() == false) {
		return;
	}

	float height_drawn = 0.0f;

	// Draw window
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, subwindow_height, UI_SUBWIN_CORNER_RADIUS);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Header
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, UI_SUBWIN_HEADER_HEIGHT, UI_SUBWIN_CORNER_RADIUS);
	nvgRect(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT / 2, subwindow_width, UI_SUBWIN_HEADER_HEIGHT / 2);
	if (is_mouse_over_header()) {
		nvgFillColor(draw_context, nvgRGBA(225, 225, 225, 255));
	}
	else {
		nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
	}
	nvgFill(draw_context);

	nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
	nvgStrokeWidth(draw_context, 0.8f);

	nvgBeginPath(draw_context);
	nvgMoveTo(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT);
	nvgLineTo(draw_context, subwindow_width, UI_SUBWIN_HEADER_HEIGHT);
	nvgStroke(draw_context);

	// Outline
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, subwindow_height, UI_SUBWIN_CORNER_RADIUS);
	nvgStroke(draw_context);

	// Title
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, subwindow_width / 2, UI_SUBWIN_HEADER_HEIGHT / 2, "Parameter Editor", nullptr);

	height_drawn += UI_SUBWIN_HEADER_HEIGHT + 1.0f;

	// Draw content here
	if (selected_param != nullptr && selected_param->socket_in_out == SocketInOut::Input)
	{
		int_input_box.displayed = false;
		float_input_box.displayed = false;
		vector_x_input_box.displayed = false;
		vector_y_input_box.displayed = false;
		vector_z_input_box.displayed = false;
		color_r_input_box.displayed = false;
		color_g_input_box.displayed = false;
		color_b_input_box.displayed = false;
		enum_targets.clear();
		bool_targets.clear();

		// Draw generic part
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgFontFace(draw_context, "sans");
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

		std::string parameter_name_text = "Parameter: " + selected_param->display_name;
		nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, parameter_name_text.c_str(), nullptr);
		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

		std::string parameter_type_text;
		switch (selected_param->socket_type) {
		case SocketType::Float:
			parameter_type_text = "Type: Float";
			break;
		case SocketType::Int:
			parameter_type_text = "Type: Integer";
			break;
		case SocketType::Color:
			parameter_type_text = "Type: Color";
			break;
		case SocketType::Vector:
			parameter_type_text = "Type: Vector";
			break;
		case SocketType::Normal:
			parameter_type_text = "Type: Normal";
			break;
		case SocketType::Closure:
			parameter_type_text = "Type: Closure";
			break;
		case SocketType::String:
			parameter_type_text = "Type: String";
			break;
		case SocketType::StringEnum:
			parameter_type_text = "Type: Enumeration";
			break;
		case SocketType::Boolean:
			parameter_type_text = "Type: Boolean";
			break;
		default:
			parameter_type_text = "Error";
			break;
		}
		nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, parameter_type_text.c_str(), nullptr);
		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
		nvgStrokeWidth(draw_context, 0.8f);

		height_drawn += 2.0f;

		const float SEPARATOR_PADDING = 8.0f;
		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, SEPARATOR_PADDING, height_drawn);
		nvgLineTo(draw_context, subwindow_width - SEPARATOR_PADDING, height_drawn);
		nvgStroke(draw_context);

		height_drawn += 4.0f;

		if (selected_param->socket_type == SocketType::Int) {
			int_input_box.displayed = true;

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			std::string value_text = "Value:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

			float input_x_draw = (2.0f * subwindow_width / 3) - (float_input_box.width / 2);
			float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - float_input_box.height) / 2;

			int_input_box.set_position(Point2(input_x_draw, input_y_draw));
			int_input_box.set_int_value(dynamic_cast<IntSocketValue*>(selected_param->value));
			int_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param->socket_type == SocketType::Float) {
			float_input_box.displayed = true;

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			std::string value_text = "Value:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

			float input_x_draw = (2.0f * subwindow_width / 3) - (float_input_box.width / 2);
			float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - float_input_box.height) / 2;

			float_input_box.set_position(Point2(input_x_draw, input_y_draw));
			float_input_box.set_float_value(dynamic_cast<FloatSocketValue*>(selected_param->value));
			float_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param->socket_type == SocketType::Vector) {
			vector_x_input_box.displayed = true;
			vector_y_input_box.displayed = true;
			vector_z_input_box.displayed = true;

			Float3SocketValue* float3_socket_val = dynamic_cast<Float3SocketValue*>(selected_param->value);

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			std::string x_label_text = "X:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, x_label_text.c_str(), nullptr);

			float input_x_draw = (2.0f * subwindow_width / 3) - (vector_x_input_box.width / 2);
			float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_x_input_box.height) / 2;

			vector_x_input_box.set_position(Point2(input_x_draw, input_y_draw));
			vector_x_input_box.set_float_value(&(float3_socket_val->x_socket_val));
			vector_x_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string y_label_text = "Y:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, y_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_y_input_box.height) / 2;

			vector_y_input_box.set_position(Point2(input_x_draw, input_y_draw));
			vector_y_input_box.set_float_value(&(float3_socket_val->y_socket_val));
			vector_y_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string z_label_text = "Z:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, z_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_z_input_box.height) / 2;

			vector_z_input_box.set_position(Point2(input_x_draw, input_y_draw));
			vector_z_input_box.set_float_value(&(float3_socket_val->z_socket_val));
			vector_z_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param->socket_type == SocketType::Color) {
			color_r_input_box.displayed = true;
			color_g_input_box.displayed = true;
			color_b_input_box.displayed = true;

			ColorSocketValue* color_socket_val = dynamic_cast<ColorSocketValue*>(selected_param->value);

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			std::string red_label_text = "R:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, red_label_text.c_str(), nullptr);

			float input_x_draw = (2.0f * subwindow_width / 3) - (color_r_input_box.width / 2);
			float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_r_input_box.height) / 2;

			color_r_input_box.set_position(Point2(input_x_draw, input_y_draw));
			color_r_input_box.set_float_value(&(color_socket_val->red_socket_val));
			color_r_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string green_label_text = "G:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, green_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_g_input_box.height) / 2;

			color_g_input_box.set_position(Point2(input_x_draw, input_y_draw));
			color_g_input_box.set_float_value(&(color_socket_val->green_socket_val));
			color_g_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string blue_label_text = "B:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, blue_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_b_input_box.height) / 2;

			color_b_input_box.set_position(Point2(input_x_draw, input_y_draw));
			color_b_input_box.set_float_value(&(color_socket_val->blue_socket_val));
			color_b_input_box.draw(draw_context, mouse_local_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param->socket_type == SocketType::StringEnum) {
			StringEnumSocketValue* str_enum_value = dynamic_cast<StringEnumSocketValue*>(selected_param->value);

			const float CHECKBOX_RADIUS = 6.0f;
			const float CHECKBOX_SPACING = 4.0f;

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			nvgTextAlign(draw_context, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

			float max_label_width = 0;

			for (StringEnumPair this_enum_value : str_enum_value->enum_values) {
				float bounds[4];
				nvgTextBounds(draw_context, 0, 0, this_enum_value.display_value.c_str(), nullptr, bounds);
				float this_width = std::ceil(bounds[2]);
				if (this_width > max_label_width) {
					max_label_width = this_width;
				}
			}

			float max_draw_width = max_label_width + CHECKBOX_SPACING + CHECKBOX_RADIUS * 2;

			for (StringEnumPair this_enum_value : str_enum_value->enum_values) {
				bool this_value_selected = (str_enum_value->value.internal_value == this_enum_value.internal_value);

				float circle_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + CHECKBOX_RADIUS;
				float circle_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.5f;
				nvgBeginPath(draw_context);
				nvgCircle(draw_context, circle_pos_x, circle_pos_y, CHECKBOX_RADIUS);
				nvgFillColor(draw_context, nvgRGBAf(0.1f, 0.1f, 0.1f, 1.0f));
				nvgFill(draw_context);
				if (this_value_selected) {
					nvgBeginPath(draw_context);
					nvgCircle(draw_context, circle_pos_x, circle_pos_y, CHECKBOX_RADIUS * 0.666f);
					nvgFillColor(draw_context, nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
					nvgFill(draw_context);
				}

				float text_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + CHECKBOX_SPACING + CHECKBOX_RADIUS * 2;
				float text_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.7f;
				nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
				nvgText(draw_context, text_pos_x, text_pos_y, this_enum_value.display_value.c_str(), nullptr);

				Point2 click_target_begin(0.0f, height_drawn);
				Point2 click_target_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				StringEnumClickTarget click_target(click_target_begin, click_target_end, this_enum_value, str_enum_value);

				enum_targets.push_back(click_target);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else if (selected_param->socket_type == SocketType::Boolean) {
			BoolSocketValue* bool_value = dynamic_cast<BoolSocketValue*>(selected_param->value);

			const float CHECKBOX_RADIUS = 6.0f;
			const float CHECKBOX_SPACING = 4.0f;

			const std::string true_label = "True";
			const std::string false_label = "False";

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			nvgTextAlign(draw_context, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

			float bounds[4];
			float true_label_width = 0.0f;
			float false_label_width = 0.0f;

			nvgTextBounds(draw_context, 0.0f, 0.0f, true_label.c_str(), nullptr, bounds);
			true_label_width = bounds[2];
			nvgTextBounds(draw_context, 0.0f, 0.0f, false_label.c_str(), nullptr, bounds);
			false_label_width = bounds[2];

			float max_label_width = std::max(true_label_width, false_label_width);
			float max_draw_width = max_label_width + CHECKBOX_SPACING + CHECKBOX_RADIUS * 2;

			for (int i = 0; i < 2; ++i) {
				std::string label;
				bool this_value_selected;
				bool current_label;
				if (i == 0) {
					label = true_label;
					this_value_selected = bool_value->value;
					current_label = true;
				}
				else {
					label = false_label;
					this_value_selected = !(bool_value->value);
					current_label = false;
				}

				float circle_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + CHECKBOX_RADIUS;
				float circle_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.5f;
				nvgBeginPath(draw_context);
				nvgCircle(draw_context, circle_pos_x, circle_pos_y, CHECKBOX_RADIUS);
				nvgFillColor(draw_context, nvgRGBAf(0.1f, 0.1f, 0.1f, 1.0f));
				nvgFill(draw_context);
				if (this_value_selected) {
					nvgBeginPath(draw_context);
					nvgCircle(draw_context, circle_pos_x, circle_pos_y, CHECKBOX_RADIUS * 0.666f);
					nvgFillColor(draw_context, nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
					nvgFill(draw_context);
				}

				float text_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + CHECKBOX_SPACING + CHECKBOX_RADIUS * 2;
				float text_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.7f;
				nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
				nvgText(draw_context, text_pos_x, text_pos_y, label.c_str(), nullptr);

				Point2 click_target_begin(0.0f, height_drawn);
				Point2 click_target_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				BoolValueClickTarget click_target(click_target_begin, click_target_end, current_label, bool_value);

				bool_targets.push_back(click_target);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else { // Unsupported type
			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
			nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, "Uneditable type selected", nullptr);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
	}

	subwindow_height = height_drawn + 4.0f;
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_mouse_button(int button, int action, int /*mods*/)
{
	if (is_mouse_over_header()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				move_window_begin();
			}
			else if (action == GLFW_RELEASE) {
				move_window_end();
			}
		}
	}
	else if (int_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&int_input_box);
		}
	}
	else if (float_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&float_input_box);
		}
	}
	else if (vector_x_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&vector_x_input_box);
		}
	}
	else if (vector_y_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&vector_y_input_box);
		}
	}
	else if (vector_z_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&vector_z_input_box);
		}
	}
	else if (color_r_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&color_r_input_box);
		}
	}
	else if (color_g_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&color_g_input_box);
		}
	}
	else if (color_b_input_box.is_mouse_over()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input(&color_b_input_box);
		}
	}
	else if (is_enum_target_under_mouse()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			click_enum_target_under_mouse();
		}
	}
	else if (is_bool_target_under_mouse()) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			click_bool_target_under_mouse();
		}
	}
	else {
		if (action == GLFW_PRESS) {
			complete_input();
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_key(int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (selected_input != nullptr) {
			selected_input->cancel_edit();
		}
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		if (selected_input != nullptr) {
			selected_input->complete_edit();
			param_changed = true;
		}
	}
	else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		if (selected_input != nullptr) {
			selected_input->backspace();
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_character(unsigned int codepoint)
{
	if (selected_input != nullptr && selected_input->should_capture_keys()) {
		selected_input->handle_character(codepoint);
	}
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_subwindow_active()
{
	return (selected_param != nullptr);
}

void CyclesShaderEditor::ParamEditorSubwindow::set_selected_param(NodeSocket* selected_param)
{
	if (this->selected_param == selected_param) {
		return;
	}
	complete_input();
	this->selected_param = selected_param;
}

bool CyclesShaderEditor::ParamEditorSubwindow::should_capture_keys()
{
	if (selected_input != nullptr) {
		return selected_input->should_capture_keys();
	}

	return false;
}

void CyclesShaderEditor::ParamEditorSubwindow::complete_input()
{
	select_input(nullptr);
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_mouse_over_header()
{
	if (subwindow_moving) {
		return true;
	}
	return (mouse_local_pos.get_pos_x() > 0 &&
		mouse_local_pos.get_pos_x() < subwindow_width &&
		mouse_local_pos.get_pos_y() > 0 &&
		mouse_local_pos.get_pos_y() < UI_SUBWIN_HEADER_HEIGHT);
}

void CyclesShaderEditor::ParamEditorSubwindow::select_input(BaseInputBox* input)
{
	if (selected_input != nullptr) {
		selected_input->complete_edit();
	}

	selected_input = input;

	if (selected_input != nullptr) {
		selected_input->begin_edit();
	}
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_bool_target_under_mouse()
{
	for (BoolValueClickTarget& this_target : bool_targets) {
		if (this_target.is_mouse_over_target(mouse_local_pos)) {
			return true;
		}
	}

	return false;
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_enum_target_under_mouse()
{
	for (StringEnumClickTarget& this_target : enum_targets) {
		if (this_target.is_mouse_over_target(mouse_local_pos)) {
			return true;
		}
	}

	return false;
}

void CyclesShaderEditor::ParamEditorSubwindow::click_bool_target_under_mouse()
{
	for (BoolValueClickTarget& this_target : bool_targets) {
		if (this_target.is_mouse_over_target(mouse_local_pos)) {
			this_target.click();
			return;
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::click_enum_target_under_mouse()
{
	for (StringEnumClickTarget& this_target : enum_targets) {
		if (this_target.is_mouse_over_target(mouse_local_pos)) {
			this_target.click();
			return;
		}
	}
}