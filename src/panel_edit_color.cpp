#include "panel_edit_color.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "gui_sizes.h"
#include "sockets.h"

CyclesShaderEditor::EditColorPanel::EditColorPanel(float width) :
	panel_width(width),
	color_r_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_g_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_b_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_BIG, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_rect_click_target(FloatPos(), FloatPos()),
	hue_bar_click_target(FloatPos(), FloatPos())
{
	// These are always displayed within this panel
	color_r_input_box.displayed = true;
	color_g_input_box.displayed = true;
	color_b_input_box.displayed = true;
}

bool CyclesShaderEditor::EditColorPanel::is_active()
{
	return active;
}

void CyclesShaderEditor::EditColorPanel::set_active(bool active)
{
	this->active = active;
}

float CyclesShaderEditor::EditColorPanel::draw(NVGcontext* draw_context, ColorSocketValue* socket_value)
{
	float height_drawn = 0.0f;

	if (socket_value == nullptr) {
		return height_drawn;
	}

	// We need to know the color value as HSV to draw the ui
	HueSatVal hsv = get_hsv();
	RedGreenBlue rgb_from_hue;
	{
		// Get RGB here, set sat and val to 1 to get the brightest color at this hue
		HueSatVal hsv_tmp = hsv;
		hsv_tmp.sat = 1.0f;
		hsv_tmp.val = 1.0f;
		rgb_from_hue = rgb_from_hsv(hsv_tmp);
	}

	// Draw color rectangle
	{
		// Background
		const float draw_x = UI_SUBWIN_PARAM_EDIT_RECT_HPAD;
		const float draw_y = height_drawn + UI_SUBWIN_PARAM_EDIT_RECT_VPAD;
		const float rect_width = panel_width - 2 * UI_SUBWIN_PARAM_EDIT_RECT_HPAD;
		const float rect_height = UI_SUBWIN_PARAM_EDIT_RECT_HEIGHT;

		NVGcolor white_color = nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
		NVGcolor black_color = nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f);
		NVGcolor hue_color = nvgRGBAf(rgb_from_hue.r, rgb_from_hue.g, rgb_from_hue.b, 1.0f);
		NVGcolor transparent_color = nvgRGBAf(0.0f, 0.0f, 0.0f, 0.0f);

		NVGpaint first_grad = nvgLinearGradient(draw_context, draw_x, draw_y, draw_x + rect_width, draw_y, white_color, hue_color);
		NVGpaint second_grad = nvgLinearGradient(draw_context, draw_x, draw_y, draw_x, draw_y + rect_height, transparent_color, black_color);

		nvgBeginPath(draw_context);
		nvgRect(draw_context, draw_x, draw_y, rect_width, rect_height);
		nvgFillPaint(draw_context, first_grad);
		nvgFill(draw_context);
		nvgFillPaint(draw_context, second_grad);
		nvgFill(draw_context);

		// Cursor
		const float cursor_pos_x = hsv.sat * rect_width + draw_x;
		const float cursor_pos_y = (1.0f - hsv.val) * rect_height + draw_y;
		Drawing::draw_color_pick_cursor(draw_context, cursor_pos_x, cursor_pos_y);

		// Click target
		FloatPos color_rect_begin = FloatPos(draw_x, draw_y);
		FloatPos color_rect_end = FloatPos(draw_x + rect_width, draw_y + rect_height);
		color_rect_click_target = GenericClickTarget(color_rect_begin, color_rect_end);

		height_drawn += 2 * UI_SUBWIN_PARAM_EDIT_RECT_VPAD + UI_SUBWIN_PARAM_EDIT_RECT_HEIGHT;
	}

	// Draw hue slider
	{
		// This is multiple gradients, color pattern is R->Y->G->T->B->M->R
		const int HUE_BAR_SEGMENT_COUNT = 6;
		const float hue_bar_width = panel_width - UI_SUBWIN_PARAM_EDIT_SLIDER_HPAD * 2;
		const float hue_bar_height = UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - UI_SUBWIN_PARAM_EDIT_SLIDER_VPAD * 2;
		const float segment_width = hue_bar_width / HUE_BAR_SEGMENT_COUNT;
		const float draw_x = UI_SUBWIN_PARAM_EDIT_SLIDER_HPAD;
		const float draw_y = height_drawn + UI_SUBWIN_PARAM_EDIT_SLIDER_VPAD;
		for (int i = 0; i < HUE_BAR_SEGMENT_COUNT; i++) {
			const float segment_draw_x = draw_x + segment_width * i;
			NVGcolor left_color = nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f);
			NVGcolor right_color = nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
			switch (i % HUE_BAR_SEGMENT_COUNT) {
			case 0:
				left_color = nvgRGBAf(1.0f, 0.0f, 0.0f, 1.0f);
				right_color = nvgRGBAf(1.0f, 1.0f, 0.0f, 1.0f);
				break;
			case 1:
				left_color = nvgRGBAf(1.0f, 1.0f, 0.0f, 1.0f);
				right_color = nvgRGBAf(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			case 2:
				left_color = nvgRGBAf(0.0f, 1.0f, 0.0f, 1.0f);
				right_color = nvgRGBAf(0.0f, 1.0f, 1.0f, 1.0f);
				break;
			case 3:
				left_color = nvgRGBAf(0.0f, 1.0f, 1.0f, 1.0f);
				right_color = nvgRGBAf(0.0f, 0.0f, 1.0f, 1.0f);
				break;
			case 4:
				left_color = nvgRGBAf(0.0f, 0.0f, 1.0f, 1.0f);
				right_color = nvgRGBAf(1.0f, 0.0f, 1.0f, 1.0f);
				break;
			case 5:
				left_color = nvgRGBAf(1.0f, 0.0f, 1.0f, 1.0f);
				right_color = nvgRGBAf(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			NVGpaint segment_bg = nvgLinearGradient(draw_context, segment_draw_x, draw_y, segment_draw_x + segment_width, draw_y, left_color, right_color);
			nvgBeginPath(draw_context);
			nvgRect(draw_context, segment_draw_x, draw_y, segment_width, hue_bar_height);
			nvgFillPaint(draw_context, segment_bg);
			nvgFill(draw_context);
		}

		// Draw slider bar on current hue value
		float hue_slider_position_x = hsv.hue * hue_bar_width + draw_x;
		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, hue_slider_position_x, height_drawn);
		nvgLineTo(draw_context, hue_slider_position_x, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
		nvgStrokeWidth(draw_context, 2.0f);
		nvgStroke(draw_context);

		FloatPos hue_bar_begin = FloatPos(draw_x, draw_y);
		FloatPos hue_bar_end = FloatPos(draw_x + hue_bar_width, draw_y + hue_bar_height);
		hue_bar_click_target = GenericClickTarget(hue_bar_begin, hue_bar_end);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	// Draw RGB input boxes
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

	std::string red_label_text = "Red:";
	nvgTextAlign(draw_context, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgText(draw_context, panel_width / 2.6f, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, red_label_text.c_str(), nullptr);

	float input_x_draw = (2.0f * panel_width / 3) - (color_r_input_box.width / 2);
	float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_r_input_box.height) / 2;

	color_r_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
	color_r_input_box.set_float_value(&(socket_value->red_socket_val));
	color_r_input_box.draw(draw_context, mouse_local_pos);

	height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

	std::string green_label_text = "Green:";
	nvgTextAlign(draw_context, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgText(draw_context, panel_width / 2.6f, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, green_label_text.c_str(), nullptr);

	input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_g_input_box.height) / 2;

	color_g_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
	color_g_input_box.set_float_value(&(socket_value->green_socket_val));
	color_g_input_box.draw(draw_context, mouse_local_pos);

	height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

	std::string blue_label_text = "Blue:";
	nvgTextAlign(draw_context, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgText(draw_context, panel_width / 2.6f, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, blue_label_text.c_str(), nullptr);

	input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - color_b_input_box.height) / 2;

	color_b_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
	color_b_input_box.set_float_value(&(socket_value->blue_socket_val));
	color_b_input_box.draw(draw_context, mouse_local_pos);

	height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

	// If color selection is active, update the color here
	if (mouse_sat_val_selection_active) {
		set_sat_val_from_mouse();
	}
	if (mouse_hue_selection_active) {
		set_hue_from_mouse();
	}

	panel_height = height_drawn;
	return height_drawn;
}

void CyclesShaderEditor::EditColorPanel::set_mouse_local_position(FloatPos local_pos)
{
	mouse_local_pos = local_pos;
}

bool CyclesShaderEditor::EditColorPanel::is_mouse_over()
{
	if (is_active() == false) {
		return false;
	}

	const float min_x = 0.0f;
	const float max_x = panel_width;
	const float min_y = 0.0f;
	const float max_y = panel_height;

	if (mouse_local_pos.get_pos_x() > min_x &&
		mouse_local_pos.get_pos_x() < max_x &&
		mouse_local_pos.get_pos_y() > min_y &&
		mouse_local_pos.get_pos_y() < max_y)
	{
		return true;
	}

	return false;
}

void CyclesShaderEditor::EditColorPanel::handle_mouse_button(int button, int action, int /*mods*/)
{
	if (color_rect_click_target.is_mouse_over_target(mouse_local_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			mouse_sat_val_selection_active = true;
		}
	}
	else if (hue_bar_click_target.is_mouse_over_target(mouse_local_pos)) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			mouse_hue_selection_active = true;
		}
	}
}

void CyclesShaderEditor::EditColorPanel::mouse_button_release()
{
	if (mouse_hue_selection_active) {
		mouse_hue_selection_active = false;
		value_has_changed = true;
	}
	if (mouse_sat_val_selection_active) {
		mouse_sat_val_selection_active = false;
		value_has_changed = true;
	}
}

CyclesShaderEditor::BaseInputBox* CyclesShaderEditor::EditColorPanel::get_input_bux_under_mouse()
{
	if (is_active() == false) {
		return nullptr;
	}

	if (color_r_input_box.is_mouse_over()) {
		return &color_r_input_box;
	}
	else if (color_g_input_box.is_mouse_over()) {
		return &color_g_input_box;
	}
	else if (color_b_input_box.is_mouse_over()) {
		return &color_b_input_box;
	}
	else {
		return nullptr;
	}
}

bool CyclesShaderEditor::EditColorPanel::should_push_undo_state()
{
	if (value_has_changed) {
		value_has_changed = false;
		return true;
	}
	return false;
}

CyclesShaderEditor::HueSatVal CyclesShaderEditor::EditColorPanel::get_hsv()
{
	RedGreenBlue rgb;
	rgb.r = color_r_input_box.get_float_value();
	rgb.g = color_g_input_box.get_float_value();
	rgb.b = color_b_input_box.get_float_value();
	HueSatVal hsv = hsv_from_rgb(rgb);
	if (hsv.hue < 0.0f) {
		// Negative hue indicates monochrome color
		// Use the most recent hue instead
		hsv.hue = last_hue;
	}
	return hsv;
}

void CyclesShaderEditor::EditColorPanel::set_hsv(HueSatVal hsv)
{
	RedGreenBlue rgb = rgb_from_hsv(hsv);

	color_r_input_box.set_float_value(rgb.r);
	color_g_input_box.set_float_value(rgb.g);
	color_b_input_box.set_float_value(rgb.b);
}

void CyclesShaderEditor::EditColorPanel::set_hue_from_mouse()
{
	float new_hue = hue_bar_click_target.get_normalized_mouse_pos(mouse_local_pos).get_pos_x();
	if (new_hue < 0.0f) {
		new_hue = 0.0f;
	}
	else if (new_hue > 1.0f) {
		new_hue = 1.0f;
	}

	HueSatVal hsv = get_hsv();
	hsv.hue = new_hue;
	set_hsv(hsv);
	last_hue = new_hue;
}

void CyclesShaderEditor::EditColorPanel::set_sat_val_from_mouse()
{
	FloatPos mouse_pos_normalized = color_rect_click_target.get_normalized_mouse_pos(mouse_local_pos);
	const float new_sat = mouse_pos_normalized.get_pos_x();
	const float new_val = 1.0f - mouse_pos_normalized.get_pos_y();

	HueSatVal hsv = get_hsv();
	hsv.sat = new_sat;
	hsv.val = new_val;
	set_hsv(hsv);
}