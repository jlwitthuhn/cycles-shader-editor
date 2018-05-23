#include "subwindow_param_editor.h"

#include <algorithm>
#include <cmath>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "input_box.h"
#include "node_base.h"

CyclesShaderEditor::ParamEditorSubwindow::ParamEditorSubwindow(FloatPos screen_position) :
	NodeEditorSubwindow(screen_position, "Parameter Editor"),
	panel_color(UI_SUBWIN_PARAM_EDIT_WIDTH),
	panel_curve(UI_SUBWIN_PARAM_EDIT_WIDTH),
	int_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	float_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_x_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_y_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_z_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT)
{
	subwindow_width = UI_SUBWIN_PARAM_EDIT_WIDTH;
}

void CyclesShaderEditor::ParamEditorSubwindow::pre_draw()
{
	if (selected_param != nullptr && selected_param->socket_type == SocketType::Curve && selected_param->value != nullptr) {
		CurveSocketValue* curve = dynamic_cast<CurveSocketValue*>(selected_param->value);
		if (curve != nullptr) {
			panel_curve.set_attached_curve_value(curve);
			panel_curve.pre_draw();
			return;
		}
	}
	panel_curve.set_attached_curve_value(nullptr);
}

void CyclesShaderEditor::ParamEditorSubwindow::set_mouse_position(FloatPos local_position, float max_pos_y)
{
	NodeEditorSubwindow::set_mouse_position(local_position, max_pos_y);
	if (panel_color.is_active()) {
		panel_color.set_mouse_local_position(mouse_panel_pos - FloatPos(0.0f, panel_start_y));
	}
	if (panel_curve.is_active()) {
		panel_curve.set_mouse_local_position(mouse_panel_pos - FloatPos(0.0f, panel_start_y));
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_mouse_button(int button, int action, int mods)
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
	else if (panel_color.is_mouse_over()) {
		if (panel_color.get_input_bux_under_mouse() != nullptr) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				select_input(panel_color.get_input_bux_under_mouse());
			}
		}
		else {
			// Panel will handle the mouse event internally
			panel_color.handle_mouse_button(button, action, mods);
		}
	}
	else if (panel_curve.is_mouse_over()) {
		panel_curve.handle_mouse_button(button, action, mods);
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
			request_undo_stack_push = true;
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

bool CyclesShaderEditor::ParamEditorSubwindow::is_active() const
{
	return (selected_param != nullptr);
}

void CyclesShaderEditor::ParamEditorSubwindow::set_selected_param(NodeSocket* selected_param)
{
	if (this->selected_param == selected_param) {
		return;
	}
	complete_input();
	panel_curve.reset_panel_state();
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

void CyclesShaderEditor::ParamEditorSubwindow::mouse_button_release()
{
	panel_color.mouse_button_release();
	panel_curve.mouse_button_release();
}

bool CyclesShaderEditor::ParamEditorSubwindow::should_push_undo_state() {
	bool result = false;
	if (request_undo_stack_push) {
		request_undo_stack_push = false;
		result = true;
	}
	if (panel_color.should_push_undo_state()) {
		result = true;
	}
	if (panel_curve.should_push_undo_state()) {
		result = true;
	}
	return result;
}

void CyclesShaderEditor::ParamEditorSubwindow::draw_content(NVGcontext* draw_context)
{
	float height_drawn = 0.0f;

	if (selected_param != nullptr && selected_param->socket_in_out == SocketInOut::Input)
	{
		int_input_box.displayed = false;
		float_input_box.displayed = false;
		vector_x_input_box.displayed = false;
		vector_y_input_box.displayed = false;
		vector_z_input_box.displayed = false;
		panel_color.set_active(false);
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
		case SocketType::Curve:
			parameter_type_text = "Type: Curve";
			break;
		default:
			parameter_type_text = "Error";
			break;
		}
		nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, parameter_type_text.c_str(), nullptr);
		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;


		// Draw separator
		height_drawn += UI_SUBWIN_PARAM_EDIT_SEPARATOR_VPAD;
		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, UI_SUBWIN_PARAM_EDIT_SEPARATOR_HPAD, height_drawn);
		nvgLineTo(draw_context, subwindow_width - UI_SUBWIN_PARAM_EDIT_SEPARATOR_HPAD, height_drawn);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
		nvgStrokeWidth(draw_context, 0.8f);
		nvgStroke(draw_context);
		height_drawn += UI_SUBWIN_PARAM_EDIT_SEPARATOR_VPAD;

		panel_start_y = height_drawn;

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

			int_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			int_input_box.set_int_value(dynamic_cast<IntSocketValue*>(selected_param->value));
			int_input_box.draw(draw_context, mouse_panel_pos);

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

			float_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			float_input_box.set_float_value(dynamic_cast<FloatSocketValue*>(selected_param->value));
			float_input_box.draw(draw_context, mouse_panel_pos);

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

			vector_x_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_x_input_box.set_float_value(&(float3_socket_val->x_socket_val));
			vector_x_input_box.draw(draw_context, mouse_panel_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string y_label_text = "Y:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, y_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_y_input_box.height) / 2;

			vector_y_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_y_input_box.set_float_value(&(float3_socket_val->y_socket_val));
			vector_y_input_box.draw(draw_context, mouse_panel_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			std::string z_label_text = "Z:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, z_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_z_input_box.height) / 2;

			vector_z_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_z_input_box.set_float_value(&(float3_socket_val->z_socket_val));
			vector_z_input_box.draw(draw_context, mouse_panel_pos);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param->socket_type == SocketType::Color) {
			nvgSave(draw_context);
			nvgTranslate(draw_context, 0.0f, panel_start_y);
			panel_color.set_active(true);
			ColorSocketValue* color_socket_val = dynamic_cast<ColorSocketValue*>(selected_param->value);
			height_drawn += panel_color.draw(draw_context, color_socket_val);
			nvgRestore(draw_context);
		}
		else if (selected_param->socket_type == SocketType::StringEnum) {
			StringEnumSocketValue* str_enum_value = dynamic_cast<StringEnumSocketValue*>(selected_param->value);

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
			float max_draw_width = max_label_width + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;

			for (StringEnumPair this_enum_value : str_enum_value->enum_values) {
				bool this_value_selected = (str_enum_value->value.internal_value == this_enum_value.internal_value);

				float circle_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_RADIUS;
				float circle_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.5f;
				nvgBeginPath(draw_context);
				nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS);
				nvgFillColor(draw_context, nvgRGBAf(0.1f, 0.1f, 0.1f, 1.0f));
				nvgFill(draw_context);
				if (this_value_selected) {
					nvgBeginPath(draw_context);
					nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS * 0.666f);
					nvgFillColor(draw_context, nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
					nvgFill(draw_context);
				}

				float text_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;
				float text_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.7f;
				nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
				nvgText(draw_context, text_pos_x, text_pos_y, this_enum_value.display_value.c_str(), nullptr);

				FloatPos click_target_begin(0.0f, height_drawn);
				FloatPos click_target_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				StringEnumClickTarget click_target(click_target_begin, click_target_end, this_enum_value, str_enum_value);

				enum_targets.push_back(click_target);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else if (selected_param->socket_type == SocketType::Boolean) {
			BoolSocketValue* bool_value = dynamic_cast<BoolSocketValue*>(selected_param->value);

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
			float max_draw_width = max_label_width + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;

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

				float circle_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_RADIUS;
				float circle_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.5f;
				nvgBeginPath(draw_context);
				nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS);
				nvgFillColor(draw_context, nvgRGBAf(0.1f, 0.1f, 0.1f, 1.0f));
				nvgFill(draw_context);
				if (this_value_selected) {
					nvgBeginPath(draw_context);
					nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS * 0.666f);
					nvgFillColor(draw_context, nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
					nvgFill(draw_context);
				}

				float text_pos_x = subwindow_width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;
				float text_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.7f;
				nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
				nvgText(draw_context, text_pos_x, text_pos_y, label.c_str(), nullptr);

				FloatPos click_target_begin(0.0f, height_drawn);
				FloatPos click_target_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				BoolValueClickTarget click_target(click_target_begin, click_target_end, current_label, bool_value);

				bool_targets.push_back(click_target);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else if (selected_param->socket_type == SocketType::Curve) {
			nvgSave(draw_context);
			nvgTranslate(draw_context, 0.0f, panel_start_y);
			CurveSocketValue* curve_socket_val = dynamic_cast<CurveSocketValue*>(selected_param->value);
			panel_curve.set_attached_curve_value(curve_socket_val);
			height_drawn += panel_curve.draw(draw_context);
			nvgRestore(draw_context);
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

	content_height = height_drawn + 4.0f;
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
		if (this_target.is_mouse_over_target(mouse_panel_pos)) {
			return true;
		}
	}

	return false;
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_enum_target_under_mouse()
{
	for (StringEnumClickTarget& this_target : enum_targets) {
		if (this_target.is_mouse_over_target(mouse_panel_pos)) {
			return true;
		}
	}

	return false;
}

void CyclesShaderEditor::ParamEditorSubwindow::click_bool_target_under_mouse()
{
	for (BoolValueClickTarget& this_target : bool_targets) {
		if (this_target.is_mouse_over_target(mouse_panel_pos)) {
			this_target.click();
			return;
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::click_enum_target_under_mouse()
{
	for (StringEnumClickTarget& this_target : enum_targets) {
		if (this_target.is_mouse_over_target(mouse_panel_pos)) {
			this_target.click();
			return;
		}
	}
}
