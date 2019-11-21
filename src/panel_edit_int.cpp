#include "panel_edit_int.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"

cse::EditIntPanel::EditIntPanel(const float width) :
	ParamEditorPanel(width),
	int_input_box(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH_SMALL, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT)
{
	int_input_box.displayed = true;
}

bool cse::EditIntPanel::is_active() const
{
	return static_cast<bool>(attached_int.lock());
}

void cse::EditIntPanel::pre_draw()
{
	if (is_active() == false) {
		return;
	}

	if (const auto attached_int_ptr = attached_int.lock()) {
		int_input_box.attach_int_value(attached_int_ptr);
	}
}

float cse::EditIntPanel::draw(NVGcontext* draw_context)
{
	float height_drawn = 0.0f;

	if (is_active() == false) {
		return height_drawn;
	}

	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

	const std::string value_text = "Value:";
	nvgText(draw_context, panel_width / 3, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

	const float input_x_draw = (2.0f * panel_width / 3) - (int_input_box.width / 2);
	const float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - int_input_box.height) / 2;

	const bool highlight = int_input_box.is_under_point(mouse_local_pos);
	int_input_box.set_position(FloatPos(input_x_draw, input_y_draw));
	int_input_box.draw(draw_context, highlight);

	height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;

	panel_height = height_drawn;
	return panel_height;
}

bool cse::EditIntPanel::should_capture_input() const
{
	return int_input_box.should_capture_input();
}

void cse::EditIntPanel::handle_mouse_button(int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (int_input_box.is_under_point(mouse_local_pos)) {
			deselect_input_box();
			int_input_box.begin_edit();
		}
		else {
			int_input_box.complete_edit();
		}
	}
}

void cse::EditIntPanel::handle_key(const int key, int /*scancode*/, const int action, int /*mods*/)
{
	if (int_input_box.should_capture_input()) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			int_input_box.cancel_edit();
		}
		else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			deselect_input_box();
		}
		else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
			int_input_box.backspace();
		}
	}
}

void cse::EditIntPanel::handle_character(const unsigned int codepoint)
{
	if (int_input_box.should_capture_input()) {
		int_input_box.handle_character(codepoint);
	}
}

void cse::EditIntPanel::set_attached_value(std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::INT) {
			const auto int_value_ptr = std::dynamic_pointer_cast<IntSocketValue>(socket_value_ptr);
			if (attached_int.lock() != int_value_ptr) {
				reset();
				attached_int = int_value_ptr;
			}
			return;
		}
	}
	attached_int = std::weak_ptr<IntSocketValue>();
}

void cse::EditIntPanel::deselect_input_box()
{
	if (int_input_box.should_capture_input()) {
		int_input_box.complete_edit();
		request_undo_push = true;
	}
}
