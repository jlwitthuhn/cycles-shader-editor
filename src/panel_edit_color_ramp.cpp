#include "panel_edit_color_ramp.h"

#include <algorithm>
#include <cmath>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "gui_sizes.h"
#include "output.h"
#include "panel_edit_color.h"
#include "sockets.h"
#include "util_color_ramp.h"
#include "util_enum.h"
#include "util_vector.h"

cse::EditColorRampPanel::EditColorRampPanel(const float width) :
	EditParamPanel(width),
	new_point_button(cse::Float2(0.0f, 0.0f), cse::Float2(1.0f, 1.0f)),
	edit_color_panel(std::make_shared<EditColorPanel>(width))
{

}

bool cse::EditColorRampPanel::is_active() const
{
	return attached_ramp.use_count() > 0;
}

void cse::EditColorRampPanel::pre_draw()
{
	// Remove any rows that have been flagged for deletion
	bool delete_happened;
	do {
		delete_happened = false;
		const auto point_count = ramp_rows.size();
		if (point_count <= 2) {
			// Deleting only works when 3 or more points exist
			break;
		}
		for (auto iter = ramp_rows.begin(); iter != ramp_rows.end(); iter++) {
			if (iter->flagged_for_deletion) {
				ramp_rows.erase(iter);
				delete_happened = true;
				request_undo_push = true;
				break;
			}
		}
	} while (delete_happened);

	// Flip everything back to not requesting deletion
	for (auto& this_row : ramp_rows) {
		this_row.flagged_for_deletion = false;
	}

	// Maybe connect selected color to color panel
	bool color_is_selected = false;
	for (const auto& this_row : ramp_rows) {
		if (this_row.color_selected) {
			edit_color_panel->set_attached_value(this_row.value_color);
			color_is_selected = true;
		}
	}
	if (color_is_selected == false) {
		edit_color_panel->set_attached_value(std::weak_ptr<SocketValue>());
	}

	// Sort all rows by position
	std::sort(
		ramp_rows.begin(),
		ramp_rows.end(),
		[](const ColorRampRow& lhs, const ColorRampRow& rhs) {
			return lhs.value_pos->get_value() < rhs.value_pos->get_value();
		}
	);

	// Copy values from internal types to the real socket
	// FIXME: This doesn't need to happen every frame, may lead to bad performance in extreme cases
	if (const auto ramp_socket_value = attached_ramp.lock()) {
		ramp_socket_value->ramp_points.clear();
		for (const auto& this_row : ramp_rows) {
			const float pos = this_row.value_pos->get_value();
			const FloatRGBColor color = this_row.value_color->get_value();
			const float alpha = this_row.value_alpha->get_value();
			const ColorRampPoint new_point(pos, Float3(color.r, color.g, color.b), alpha);
			ramp_socket_value->ramp_points.push_back(new_point);
		}
	}

	edit_color_panel->pre_draw();
}

float cse::EditColorRampPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	const float point_width = panel_width - UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD * 2;

	const float x_left =   UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width / 4.0f;
	const float x_middle = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width / 2.0f;
	const float x_right =  UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width * 3.0f / 4.0f;

	// Header
	{
		const float y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2;

		nvgText(draw_context, x_left, y, "Pos", nullptr);
		nvgText(draw_context, x_middle, y, "Color", nullptr);
		nvgText(draw_context, x_right, y, "Alpha", nullptr);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	// Calculate positions for all buttons
	const float y_box = std::floor((UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT) / 2.0f);
	const float x_left_box = std::floor(x_left - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));
	const float x_right_box = std::floor(x_right - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));
	const float x_middle_box = std::floor(x_middle - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));

	for (ColorRampRow& this_row : ramp_rows) {
		const float x_rect = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float y_rect = height_drawn + UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const float width = panel_width - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float height = UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;

		// Draw background rectangle for this row
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, x_rect, y_rect, width, height, UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_CORNER_RADIUS);
		nvgFillColor(draw_context, nvgRGBA(130, 130, 130, 255));
		nvgFill(draw_context);

		// Draw delete button
		const bool allow_delete = (ramp_rows.size() > 2);
		if (allow_delete) {
			constexpr float BUTTON_SIZE = 10.0f;
			constexpr float BUTTON_HPAD = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
			const float min_x = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + BUTTON_HPAD;
			const float max_x = min_x + BUTTON_SIZE;
			const float min_y = height_drawn + (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - BUTTON_SIZE) / 2.0f;
			const float max_y = min_y + BUTTON_SIZE;
			nvgBeginPath(draw_context);
			nvgMoveTo(draw_context, min_x, min_y);
			nvgLineTo(draw_context, max_x, max_y);
			nvgMoveTo(draw_context, max_x, min_y);
			nvgLineTo(draw_context, min_x, max_y);
			nvgStrokeColor(draw_context, nvgRGB(240, 80, 80));
			nvgStrokeWidth(draw_context, 2.0f);
			nvgStroke(draw_context);
			this_row.delete_target = Area(Float2(min_x, min_y), Float2(max_x, max_y));
		}
		else {
			this_row.delete_target = Area(Float2(), Float2());
		}

		// Draw input boxes
		{
			const Float2 left_box_pos(x_left_box, y_box + height_drawn);
			const bool highlight_left = this_row.box_pos.contains_point(mouse_local_pos);
			this_row.box_pos.set_position(left_box_pos);
			this_row.box_pos.draw(draw_context, highlight_left);

			const Float2 right_box_pos(x_right_box, y_box + height_drawn);
			const bool highlight_right = this_row.box_alpha.contains_point(mouse_local_pos);
			this_row.box_alpha.set_position(right_box_pos);
			this_row.box_alpha.draw(draw_context, highlight_right);
		}

		// Draw color swatch
		{
			const FloatRGBColor row_color = this_row.value_color->get_value();
			const Float3 nvg_row_color(row_color.r, row_color.g, row_color.b);
			const Float2 swatch_pos(x_middle_box, y_box + height_drawn);
			const float swatch_width = UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH;
			const float swatch_height = UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT;
			Drawing::draw_color_swatch(draw_context, swatch_pos, swatch_width, swatch_height, nvg_row_color, this_row.color_selected);
			const Float2 swatch_end(swatch_pos.x + swatch_width, swatch_pos.y + swatch_height);
			this_row.color_target = Area(swatch_pos, swatch_end);
		}

		height_drawn += UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT;
	}

	// Draw new row button
	{
		const float button_pos_x = 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float button_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const Float2 button_pos(button_pos_x, button_pos_y);
		const float button_width = panel_width - 4 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float button_height = UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const Float2 button_end(button_pos_x + button_width, button_pos_y + button_height);
		new_point_button = Area(button_pos, button_end);
		Drawing::draw_button(draw_context, button_pos, button_width, button_height, "+", true, new_point_button_pressed);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	// Draw edit color panel
	if (edit_color_panel->is_active()) {
		edit_color_panel_pos = height_drawn;
		nvgSave(draw_context);
		nvgTranslate(draw_context, 0.0f, height_drawn);
		height_drawn += edit_color_panel->draw(draw_context);
		nvgRestore(draw_context);
	}

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditColorRampPanel::set_mouse_local_position(const Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 exit_color_mouse_pos(local_pos.x, local_pos.y - edit_color_panel_pos);
	edit_color_panel->set_mouse_local_position(exit_color_mouse_pos);
}

bool cse::EditColorRampPanel::should_capture_input() const
{
	bool result = false;
	for (const auto& this_row : ramp_rows) {
		result = this_row.box_pos.should_capture_input() || result;
		result = this_row.box_alpha.should_capture_input() || result;
	}
	result = edit_color_panel->should_capture_input() || result;
	return result && is_active();
}

void cse::EditColorRampPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		deselect_input_box();
		for (auto& this_row : ramp_rows) {
			if (this_row.box_pos.contains_point(mouse_local_pos)) {
				this_row.box_pos.begin_edit();
			}
			if (this_row.box_alpha.contains_point(mouse_local_pos)) {
				this_row.box_alpha.begin_edit();
			}
			if (this_row.color_target.contains_point(mouse_local_pos)) {
				this_row.color_selected = true;
			}
			if (this_row.delete_target.contains_point(mouse_local_pos)) {
				this_row.flagged_for_deletion = true;
			}
		}
		if (new_point_button.contains_point(mouse_local_pos)) {
			new_point_button_pressed = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (new_point_button.contains_point(mouse_local_pos) && new_point_button_pressed) {
			ColorRampPoint new_point(1.0f, Float3(1.0f, 1.0f, 1.0f), 1.0f);
			ramp_rows.push_back(new_point);
		}
		new_point_button_pressed = false;
	}

	edit_color_panel->handle_mouse_button(button, action, mods);
}

void cse::EditColorRampPanel::handle_key(const int key, const int scancode, const int action, const int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			this_row.box_pos.cancel_edit();
			this_row.box_alpha.cancel_edit();
		}
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			request_undo_push = this_row.box_pos.complete_edit() || request_undo_push;
			request_undo_push = this_row.box_alpha.complete_edit() || request_undo_push;
		}
	}
	else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			this_row.box_pos.backspace();
			this_row.box_alpha.backspace();
		}
	}

	if (edit_color_panel->should_capture_input()) {
		edit_color_panel->handle_key(key, scancode, action, mods);
	}
}

void cse::EditColorRampPanel::handle_character(const unsigned int codepoint)
{
	for (auto& this_row : ramp_rows) {
		if (this_row.box_pos.should_capture_input()) {
			this_row.box_pos.handle_character(codepoint);
		}
		if (this_row.box_alpha.should_capture_input()) {
			this_row.box_alpha.handle_character(codepoint);
		}
	}

	if (edit_color_panel->should_capture_input()) {
		edit_color_panel->handle_character(codepoint);
	}
}

void cse::EditColorRampPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::COLOR_RAMP) {
			const auto ramp_value_ptr = std::dynamic_pointer_cast<ColorRampSocketValue>(socket_value_ptr);
			if (attached_ramp.lock() != ramp_value_ptr) {
				attached_ramp = ramp_value_ptr;
				ramp_rows.clear();
				for (const auto& this_point : ramp_value_ptr->ramp_points) {
					ramp_rows.push_back(this_point);
				}
			}
			return;
		}
	}
	attached_ramp = std::weak_ptr<ColorRampSocketValue>();
	ramp_rows.clear();
}

void cse::EditColorRampPanel::deselect_input_box()
{
	for (auto& this_row : ramp_rows) {
		request_undo_push = this_row.box_pos.complete_edit() || request_undo_push;
		request_undo_push = this_row.box_alpha.complete_edit() || request_undo_push;
		if (edit_color_panel->is_mouse_over() == false) {
			this_row.color_selected = false;
		}
	}
}

bool cse::EditColorRampPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = edit_color_panel->should_push_undo_state() || result;
	return result;
}

cse::EditColorRampPanel::ColorRampRow::ColorRampRow(const cse::ColorRampPoint point) :
	box_pos(UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	box_alpha(UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_target(Float2(0.0f, 0.0f), Float2(0.0f, 0.0f)),
	delete_target(Float2(0.0f, 0.0f), Float2(0.0f, 0.0f)),
	value_pos(std::make_shared<FloatSocketValue>(point.position, 0.0f, 1.0f)),
	value_alpha(std::make_shared<FloatSocketValue>(point.alpha, 0.0f, 1.0f)),
	value_color(std::make_shared<ColorSocketValue>(point.color.x, point.color.y, point.color.z))
{
	box_pos.attach_float_value(value_pos);
	box_alpha.attach_float_value(value_alpha);
}
