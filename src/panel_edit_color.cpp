#include "panel_edit_color.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "gui_sizes.h"
#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"
#include "wrapper_nvg_func.h"

cse::EditColorPanel::EditColorPanel(const float width) :
	EditParamPanel(width),
	color_rect_click_target(Float2(), Float2()),
	hue_bar_click_target(Float2(), Float2()),
	input_widget(width)
{

}

bool cse::EditColorPanel::is_active() const
{
	return static_cast<bool>(attached_color.lock());
}

void cse::EditColorPanel::pre_draw()
{
	if (is_active() == false) {
		return;
	}

	// If color selection is active, update the color here
	if (mouse_sat_val_selection_active) {
		set_sat_val_from_mouse();
	}
	if (mouse_hue_selection_active) {
		set_hue_from_mouse();
	}

	const Float3 hsv = get_hsv();
	const float hue = hsv.x;
	if (hue >= 0.0f) {
		last_hue = hue;
	}
}

float cse::EditColorPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	if (is_active() == false) {
		return height_drawn;
	}

	const auto attached_color_ptr = attached_color.lock();

	// We need to know the color value as HSV to draw the ui
	// Get RGB here, set sat and val to 1 to get the brightest color at this hue
	const Float3 hsv_tmp(last_hue, 1.0f, 1.0f);
	const Float3 hue_color = hsv_tmp.hsv_as_rgb();

	// Add a little top margin
	height_drawn += 6.0f;

	// Draw color rectangle
	{
		// Background
		const float draw_x = UI_SUBWIN_PARAM_EDIT_RECT_HPAD;
		const float draw_y = height_drawn + UI_SUBWIN_PARAM_EDIT_RECT_VPAD;
		const float rect_width = panel_width - 2 * UI_SUBWIN_PARAM_EDIT_RECT_HPAD;
		const float rect_height = UI_SUBWIN_PARAM_EDIT_RECT_HEIGHT;

		const Float3 white_color(1.0f, 1.0f, 1.0f);

		NVGcolor black_color = nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f);
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
		const Float3 hsv = get_hsv();
		const float cursor_pos_x = hsv.y * rect_width + draw_x;
		const float cursor_pos_y = (1.0f - hsv.z) * rect_height + draw_y;
		Drawing::draw_color_pick_cursor(draw_context, Float2(cursor_pos_x, cursor_pos_y));

		// Click target
		Float2 color_rect_begin = Float2(draw_x, draw_y);
		Float2 color_rect_end = Float2(draw_x + rect_width, draw_y + rect_height);
		color_rect_click_target = Area(color_rect_begin, color_rect_end);

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
		float hue_slider_position_x = last_hue * hue_bar_width + draw_x;
		nvgBeginPath(draw_context);
		nvgMoveTo(draw_context, hue_slider_position_x, height_drawn);
		nvgLineTo(draw_context, hue_slider_position_x, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
		nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 1.0f));
		nvgStrokeWidth(draw_context, 2.0f);
		nvgStroke(draw_context);

		Float2 hue_bar_begin = Float2(draw_x, draw_y);
		Float2 hue_bar_end = Float2(draw_x + hue_bar_width, draw_y + hue_bar_height);
		hue_bar_click_target = Area(hue_bar_begin, hue_bar_end);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	nvgSave(draw_context);
	nvgTranslate(draw_context, 0.0f, height_drawn);
	input_widget_pos = height_drawn;
	height_drawn += input_widget.draw(draw_context);
	nvgRestore(draw_context);

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditColorPanel::set_mouse_local_position(Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 input_widget_offset = local_pos - Float2(0.0f, input_widget_pos);
	input_widget.set_mouse_local_position(input_widget_offset);
}

bool cse::EditColorPanel::should_capture_input() const
{
	return input_widget.should_capture_input();
}

void cse::EditColorPanel::handle_mouse_button(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (color_rect_click_target.contains_point(mouse_local_pos)) {
			mouse_sat_val_selection_active = true;
		}
		else if (hue_bar_click_target.contains_point(mouse_local_pos)) {
			mouse_hue_selection_active = true;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// End any click+drag in progress
		if (mouse_hue_selection_active) {
			mouse_hue_selection_active = false;
			request_undo_push = true;
		}
		if (mouse_sat_val_selection_active) {
			mouse_sat_val_selection_active = false;
			request_undo_push = true;
		}
	}

	input_widget.handle_mouse_button(button, action, mods);
}

void cse::EditColorPanel::handle_key(const int key, int scancode, const int action, int mods)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_key(key, scancode, action, mods);
	}
}

void cse::EditColorPanel::handle_character(const unsigned int codepoint)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_character(codepoint);
	}
}

void cse::EditColorPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::COLOR) {
			const auto color_value_ptr = std::dynamic_pointer_cast<ColorSocketValue>(socket_value_ptr);
			const auto attached_color_ptr = attached_color.lock();
			if (attached_color_ptr != color_value_ptr) {
				if (attached_color_ptr) {
					attached_color_ptr->last_hue = last_hue;
				}
				last_hue = color_value_ptr->last_hue;
				attached_color = color_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("Red:", color_value_ptr->r_socket_val);
				input_widget.add_socket_input("Green:", color_value_ptr->g_socket_val);
				input_widget.add_socket_input("Blue:", color_value_ptr->b_socket_val);
			}
			return;
		}
	}
	attached_color = std::weak_ptr<ColorSocketValue>();
	input_widget.clear_sockets();
}

void cse::EditColorPanel::deselect_input_box()
{
	request_undo_push = input_widget.complete_input() || request_undo_push;
}

bool cse::EditColorPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = input_widget.should_push_undo_state() || result;
	return result;
}

cse::Float3 cse::EditColorPanel::get_hsv()
{
	Float3 rgb;
	if (const auto locked = attached_color.lock()) {
		rgb.x = locked->r_socket_val->get_value();
		rgb.y = locked->g_socket_val->get_value();
		rgb.z = locked->b_socket_val->get_value();
	}
	const Float3 hsv = rgb.rgb_as_hsv();
	return hsv;
}

void cse::EditColorPanel::set_hsv(const Float3 hsv)
{
	const Float3 rgb = hsv.hsv_as_rgb();

	if (const auto locked = attached_color.lock()) {
		locked->r_socket_val->set_value(rgb.x);
		locked->g_socket_val->set_value(rgb.y);
		locked->b_socket_val->set_value(rgb.z);
	}
}

void cse::EditColorPanel::set_hue_from_mouse()
{
	float new_hue = hue_bar_click_target.get_normalized_pos(mouse_local_pos).x;
	if (new_hue < 0.0f) {
		new_hue = 0.0f;
	}
	else if (new_hue >= 1.0f) {
		new_hue = 0.0f;
	}

	Float3 hsv = get_hsv();
	hsv.x = new_hue;
	last_hue = new_hue;
	set_hsv(hsv);
}

void cse::EditColorPanel::set_sat_val_from_mouse()
{
	const Float2 mouse_pos_normalized = color_rect_click_target.get_normalized_pos(mouse_local_pos);
	const float new_sat = mouse_pos_normalized.x;
	const float new_val = 1.0f - mouse_pos_normalized.y;

	Float3 hsv = get_hsv();
	if (hsv.x < 0.0f) {
		hsv.x = last_hue;
	}
	hsv.y = new_sat;
	hsv.z = new_val;
	set_hsv(hsv);
}
