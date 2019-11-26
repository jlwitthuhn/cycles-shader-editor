#include "panel_edit_float.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"

cse::EditFloatPanel::EditFloatPanel(const float width) :
	EditParamPanel(width),
	input_widget(width)
{

}

bool cse::EditFloatPanel::is_active() const
{
	return static_cast<bool>(attached_float.lock());
}

float cse::EditFloatPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	if (is_active() == false) {
		return height_drawn;
	}

	nvgSave(draw_context);
	nvgTranslate(draw_context, 0.0f, height_drawn);
	input_widget_pos = height_drawn;
	height_drawn += input_widget.draw(draw_context);
	nvgRestore(draw_context);

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditFloatPanel::set_mouse_local_position(const FloatPos local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const FloatPos input_widget_offset = local_pos - FloatPos(0.0f, input_widget_pos);
	input_widget.set_mouse_local_position(input_widget_offset);
}

bool cse::EditFloatPanel::should_capture_input() const
{
	return input_widget.should_capture_input();
}

void cse::EditFloatPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	input_widget.handle_mouse_button(button, action, mods);
}

void cse::EditFloatPanel::handle_key(const int key, int scancode, const int action, const int mods)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_key(key, scancode, action, mods);
	}
}

void cse::EditFloatPanel::handle_character(const unsigned int codepoint)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_character(codepoint);
	}
}

void cse::EditFloatPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::FLOAT) {
			const auto float_value_ptr = std::dynamic_pointer_cast<FloatSocketValue>(socket_value_ptr);
			if (attached_float.lock() != float_value_ptr) {
				attached_float = float_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("Value:", attached_float);
			}
			return;
		}
	}
	attached_float = std::weak_ptr<FloatSocketValue>();
	input_widget.clear_sockets();
}

void cse::EditFloatPanel::deselect_input_box()
{
	request_undo_push = input_widget.complete_input() || request_undo_push;
}

bool cse::EditFloatPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = input_widget.should_push_undo_state() || result;
	return result;
}
