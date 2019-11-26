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
#include "panel_edit_float.h"
#include "panel_edit_int.h"
#include "panel_edit_vector.h"
#include "selection.h"
#include "sockets.h"

cse::ParamEditorSubwindow::ParamEditorSubwindow(const FloatPos screen_position) :
	NodeEditorSubwindow(screen_position, "Parameter Editor")
{
	subwindow_width = UI_SUBWIN_PARAM_EDIT_WIDTH;
	panels.push_back(std::make_shared<EditColorPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditCurvePanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditFloatPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditIntPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditVectorPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
}

void cse::ParamEditorSubwindow::pre_draw()
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

void cse::ParamEditorSubwindow::set_mouse_position(const FloatPos local_position, const float max_pos_y)
{
	NodeEditorSubwindow::set_mouse_position(local_position, max_pos_y);
	for (const auto& this_panel : panels) {
		if (this_panel->is_active()) {
			this_panel->set_mouse_local_position(mouse_content_pos - FloatPos(0.0f, panel_start_y));
		}
	}
}

void cse::ParamEditorSubwindow::handle_mouse_button(const int button, const int action, const int mods)
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
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			deselect_input_box();
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		for (const auto& this_panel : panels) {
			this_panel->handle_mouse_button(button, action, mods);
		}
	}
}

void cse::ParamEditorSubwindow::handle_key(const int key, const int scancode, const int action, const int mods)
{
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			this_panel->handle_key(key, scancode, action, mods);
		}
	}
}

void cse::ParamEditorSubwindow::handle_character(const unsigned int codepoint)
{
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			this_panel->handle_character(codepoint);
		}
	}
}

void cse::ParamEditorSubwindow::deselect_input_box()
{
	for (const auto& this_panel : panels) {
		this_panel->deselect_input_box();
	}
}

bool cse::ParamEditorSubwindow::is_active() const
{
	return (selected_param.use_count() > 0);
}

bool cse::ParamEditorSubwindow::needs_undo_push()
{
	bool result = request_undo_stack_push;
	request_undo_stack_push = false;

	for (const auto& this_panel : panels) {
		result = this_panel->should_push_undo_state() || result;
	}
	return result;
}

void cse::ParamEditorSubwindow::update_selection(const std::weak_ptr<const Selection> selection)
{
	const auto selection_ptr = selection.lock();
	if (selection_ptr) {
		if (selected_param.lock() != selection_ptr->socket.lock()) {
			deselect_input_box();
			selected_param = selection_ptr->socket;
			content_height = 0.0f;
		}
	}
}

bool cse::ParamEditorSubwindow::should_capture_input() const
{
	for (const auto& this_panel : panels) {
		if (this_panel->should_capture_input()) {
			return true;
		}
	}
	return false;
}

void cse::ParamEditorSubwindow::draw_content(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	auto selected_param_ptr = selected_param.lock();
	if (selected_param_ptr && selected_param_ptr->io_type == SocketIOType::INPUT) {
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

		if (selected_param_ptr->socket_type == SocketType::STRING_ENUM) {
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
					nvgScissor(draw_context, 0.0f, 0.0f, this_panel->get_width(), this_panel->get_height());
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

bool cse::ParamEditorSubwindow::is_bool_target_under_mouse()
{
	for (BoolValueArea& this_target : bool_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			return true;
		}
	}

	return false;
}

bool cse::ParamEditorSubwindow::is_enum_target_under_mouse()
{
	for (StringEnumArea& this_target : enum_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			return true;
		}
	}

	return false;
}

void cse::ParamEditorSubwindow::click_bool_target_under_mouse()
{
	for (BoolValueArea& this_target : bool_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			this_target.click();
			return;
		}
	}
}

void cse::ParamEditorSubwindow::click_enum_target_under_mouse()
{
	for (StringEnumArea& this_target : enum_targets) {
		if (this_target.is_under_point(mouse_content_pos)) {
			this_target.click();
			return;
		}
	}
}
