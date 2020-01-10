#include "subwindow_param_editor.h"

#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "panel_edit.h"
#include "panel_edit_bool.h"
#include "panel_edit_color.h"
#include "panel_edit_color_ramp.h"
#include "panel_edit_curve.h"
#include "panel_edit_enum.h"
#include "panel_edit_multi_input.h"
#include "selection.h"
#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::ParamEditorSubwindow::ParamEditorSubwindow(const Float2 screen_position) :
	NodeEditorSubwindow(screen_position, "Parameter Editor")
{
	subwindow_width = UI_SUBWIN_PARAM_EDIT_WIDTH;
	panels.push_back(std::make_shared<EditBoolPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditColorPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditColorRampPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditCurvePanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditEnumPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
	panels.push_back(std::make_shared<EditMultiInputPanel>(UI_SUBWIN_PARAM_EDIT_WIDTH));
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

void cse::ParamEditorSubwindow::set_mouse_position(const Float2 local_position, const float max_pos_y)
{
	NodeEditorSubwindow::set_mouse_position(local_position, max_pos_y);
	for (const auto& this_panel : panels) {
		if (this_panel->is_active()) {
			this_panel->set_mouse_local_position(mouse_content_pos - Float2(0.0f, panel_start_y));
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
	else {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			deselect_input_box();
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		for (const auto& this_panel : panels) {
			if (this_panel->is_active()) {
				this_panel->handle_mouse_button(button, action, mods);
			}
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
		case SocketType::COLOR_RAMP:
			parameter_type_text = "Type: Color Ramp";
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

	content_height = height_drawn + 4.0f;
}
