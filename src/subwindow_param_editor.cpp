#include "subwindow_param_editor.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "common_enums.h"
#include "float_pos.h"
#include "gui_sizes.h"
#include "input_box.h"
#include "panel_edit.h"
#include "panel_edit_color.h"
#include "panel_edit_curve.h"
#include "selection.h"
#include "sockets.h"

CyclesShaderEditor::ParamEditorSubwindow::ParamEditorSubwindow(FloatPos screen_position) :
	NodeEditorSubwindow(screen_position, "Parameter Editor"),
	int_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	float_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_x_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_y_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	vector_z_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT)
{
	subwindow_width = UI_SUBWIN_PARAM_EDIT_WIDTH;
	panels.push_back(std::make_shared<EditColorPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditCurvePanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
}

void CyclesShaderEditor::ParamEditorSubwindow::pre_draw()
{
	if (auto selected_param_ptr = selected_param.lock()) {
		for (const auto& this_panel : panels) {
			this_panel->set_attached_value(selected_param_ptr->value);
		}
	}
	for (const auto& this_panel : panels) {
		this_panel->pre_draw();
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::set_mouse_position(FloatPos local_position, float max_pos_y)
{
	NodeEditorSubwindow::set_mouse_position(local_position, max_pos_y);
	for (const auto& this_panel : panels) {
		if (this_panel->is_active()) {
			this_panel->set_mouse_local_position(mouse_content_pos - FloatPos(0.0f, panel_start_y));
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_mouse_button(int button, int action, int mods)
{
	bool mouse_is_over_panel = false;
	for (const auto& this_panel : panels) {
		if (this_panel->is_active() && this_panel->is_mouse_over()) {
			mouse_is_over_panel = true;
		}
	}

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
	else if (mouse_is_over_panel) {
		for (const auto& this_panel : panels) {
			if (this_panel->is_active() && this_panel->is_mouse_over()) {
				this_panel->handle_mouse_button(button, action, mods);
			}
		}
	}
	else if (int_input_box.is_under_point(mouse_content_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input_box(&int_input_box);
		}
	}
	else if (float_input_box.is_under_point(mouse_content_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input_box(&float_input_box);
		}
	}
	else if (vector_x_input_box.is_under_point(mouse_content_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input_box(&vector_x_input_box);
		}
	}
	else if (vector_y_input_box.is_under_point(mouse_content_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input_box(&vector_y_input_box);
		}
	}
	else if (vector_z_input_box.is_under_point(mouse_content_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			select_input_box(&vector_z_input_box);
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
			deselect_input_box();
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		for (const auto& this_panel : panels) {
			this_panel->handle_mouse_button(button, action, mods);
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_key(const int key, const int scancode, const int action, const int mods)
{
	if (selected_input != nullptr) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			selected_input->cancel_edit();
		}
		else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			selected_input->complete_edit();
			request_undo_stack_push = true;
		}
		else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
			selected_input->backspace();
		}
	}
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			this_panel->handle_key(key, scancode, action, mods);
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::handle_character(unsigned int codepoint)
{
	if (selected_input != nullptr) {
		if (selected_input->should_capture_input()) {
			selected_input->handle_character(codepoint);
		}
	}
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			this_panel->handle_character(codepoint);
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::deselect_input_box()
{
	select_input_box(nullptr);
	for (const auto& this_panel : panels) {
		this_panel->deselect_input_box();
	}
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_active() const
{
	return (selected_param.use_count() > 0);
}

bool CyclesShaderEditor::ParamEditorSubwindow::needs_undo_push()
{
	bool result = false;
	if (request_undo_stack_push) {
		request_undo_stack_push = false;
		result = true;
	}
	for (const auto& this_panel : panels) {
		if (this_panel->should_push_undo_state()) {
			result = true;
		}
	}
	return result;
}

void CyclesShaderEditor::ParamEditorSubwindow::update_selection(const std::weak_ptr<const Selection> selection)
{
	const auto selection_ptr = selection.lock();
	if (selection_ptr) {
		if (selected_param.lock() != selection_ptr->socket.lock()) {
			deselect_input_box();
			selected_param = selection_ptr->socket;
		}
	}
}

bool CyclesShaderEditor::ParamEditorSubwindow::should_capture_input() const
{
	if (selected_input != nullptr && selected_input->should_capture_input()) {
		return true;
	}
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			return true;
		}
	}
	return false;
}

void CyclesShaderEditor::ParamEditorSubwindow::draw_content(NVGcontext* draw_context)
{
	float height_drawn = 0.0f;

	auto selected_param_ptr = selected_param.lock();
	if (selected_param_ptr && selected_param_ptr->io_type == SocketIOType::INPUT) {
		int_input_box.displayed = false;
		float_input_box.displayed = false;
		vector_x_input_box.displayed = false;
		vector_y_input_box.displayed = false;
		vector_z_input_box.displayed = false;
		enum_targets.clear();
		bool_targets.clear();

		// Draw generic part
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgFontFace(draw_context, "sans");
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

		std::string parameter_name_text = "Parameter: " + selected_param_ptr->display_name;
		nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, parameter_name_text.c_str(), nullptr);
		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

		std::string parameter_type_text;
		switch (selected_param_ptr->socket_type) {
		case SocketType::FLOAT:
			parameter_type_text = "Type: Float";
			break;
		case SocketType::INT:
			parameter_type_text = "Type: Integer";
			break;
		case SocketType::COLOR:
			parameter_type_text = "Type: Color";
			break;
		case SocketType::VECTOR:
			parameter_type_text = "Type: Vector";
			break;
		case SocketType::NORMAL:
			parameter_type_text = "Type: Normal";
			break;
		case SocketType::CLOSURE:
			parameter_type_text = "Type: Closure";
			break;
		case SocketType::STRING:
			parameter_type_text = "Type: String";
			break;
		case SocketType::STRING_ENUM:
			parameter_type_text = "Type: Enumeration";
			break;
		case SocketType::BOOLEAN:
			parameter_type_text = "Type: Boolean";
			break;
		case SocketType::CURVE:
			parameter_type_text = "Type: Curve";
			break;
		default:
			parameter_type_text = "Type: Error";
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

		if (selected_param_ptr->socket_type == SocketType::INT) {
			int_input_box.displayed = true;

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			const std::string value_text = "Value:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

			const float input_x_draw = (2.0f * subwindow_width / 3) - (float_input_box.width / 2);
			const float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - float_input_box.height) / 2;

			const bool highlight = int_input_box.is_under_point(mouse_content_pos);
			int_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			int_input_box.set_int_value(std::dynamic_pointer_cast<IntSocketValue>(selected_param_ptr->value));
			int_input_box.draw(draw_context, highlight);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param_ptr->socket_type == SocketType::FLOAT) {
			float_input_box.displayed = true;

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			const std::string value_text = "Value:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

			const float input_x_draw = (2.0f * subwindow_width / 3) - (float_input_box.width / 2);
			const float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - float_input_box.height) / 2;

			const bool highlight = float_input_box.is_under_point(mouse_content_pos);
			float_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			float_input_box.set_float_value(std::dynamic_pointer_cast<FloatSocketValue>(selected_param_ptr->value));
			float_input_box.draw(draw_context, highlight);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param_ptr->socket_type == SocketType::VECTOR) {
			vector_x_input_box.displayed = true;
			vector_y_input_box.displayed = true;
			vector_z_input_box.displayed = true;

			const std::shared_ptr<Float3SocketValue> float3_socket_val = std::dynamic_pointer_cast<Float3SocketValue>(selected_param_ptr->value);

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			const std::string x_label_text = "X:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, x_label_text.c_str(), nullptr);

			const float input_x_draw = (2.0f * subwindow_width / 3) - (vector_x_input_box.width / 2);
			float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_x_input_box.height) / 2;

			const bool highlight_x = vector_x_input_box.is_under_point(mouse_content_pos);
			vector_x_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_x_input_box.set_float_value(float3_socket_val->x_socket_val);
			vector_x_input_box.draw(draw_context, highlight_x);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			const std::string y_label_text = "Y:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, y_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_y_input_box.height) / 2;

			const bool highlight_y = vector_y_input_box.is_under_point(mouse_content_pos);
			vector_y_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_y_input_box.set_float_value(float3_socket_val->y_socket_val);
			vector_y_input_box.draw(draw_context, highlight_y);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

			const std::string z_label_text = "Z:";
			nvgText(draw_context, subwindow_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, z_label_text.c_str(), nullptr);

			input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - vector_z_input_box.height) / 2;

			const bool highlight_z = vector_z_input_box.is_under_point(mouse_content_pos);
			vector_z_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
			vector_z_input_box.set_float_value(float3_socket_val->z_socket_val);
			vector_z_input_box.draw(draw_context, highlight_z);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
		else if (selected_param_ptr->socket_type == SocketType::STRING_ENUM) {
			const auto str_enum_value = std::dynamic_pointer_cast<StringEnumSocketValue>(selected_param_ptr->value);

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

				FloatPos click_area_begin(0.0f, height_drawn);
				FloatPos click_area_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				StringEnumArea click_area(click_area_begin, click_area_end, this_enum_value, str_enum_value);

				enum_targets.push_back(click_area);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else if (selected_param_ptr->socket_type == SocketType::BOOLEAN) {
			const auto bool_value = std::dynamic_pointer_cast<BoolSocketValue>(selected_param_ptr->value);

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

				FloatPos click_area_begin(0.0f, height_drawn);
				FloatPos click_area_end(subwindow_width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
				BoolValueArea click_area(click_area_begin, click_area_end, current_label, bool_value);

				bool_targets.push_back(click_area);

				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
		else {
			// No built-in support for this type, it is handled in a panel or is an uneditable type
			bool ui_drawn = false;
			for (const auto& this_panel : panels) {
				if (this_panel->is_active()) {
					nvgSave(draw_context);
					nvgTranslate(draw_context, 0.0f, panel_start_y);
					height_drawn += this_panel->draw(draw_context);
					nvgRestore(draw_context);
					ui_drawn = true;
					break;
				}
			}
			if (ui_drawn == false) {
				nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
				nvgFontFace(draw_context, "sans");
				nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
				nvgFontBlur(draw_context, 0.0f);
				nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
				nvgText(draw_context, subwindow_width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, "Uneditable type selected", nullptr);
				height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
			}
		}
	}

	content_height = height_drawn + 4.0f;
}

void CyclesShaderEditor::ParamEditorSubwindow::select_input_box(BaseInputBox* const input)
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
	for (BoolValueArea& this_target : bool_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			return true;
		}
	}

	return false;
}

bool CyclesShaderEditor::ParamEditorSubwindow::is_enum_target_under_mouse()
{
	for (StringEnumArea& this_target : enum_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			return true;
		}
	}

	return false;
}

void CyclesShaderEditor::ParamEditorSubwindow::click_bool_target_under_mouse()
{
	for (BoolValueArea& this_target : bool_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			this_target.click();
			return;
		}
	}
}

void CyclesShaderEditor::ParamEditorSubwindow::click_enum_target_under_mouse()
{
	for (StringEnumArea& this_target : enum_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			this_target.click();
			return;
		}
	}
}
