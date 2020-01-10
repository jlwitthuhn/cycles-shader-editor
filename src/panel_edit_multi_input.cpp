#include "panel_edit_multi_input.h"

#include <nanovg.h>

#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::EditMultiInputPanel::EditMultiInputPanel(const float width) :
	EditParamPanel(width),
	input_widget(width)
{

}

bool cse::EditMultiInputPanel::is_active() const
{
	bool result = false;
	result = (attached_int.use_count() > 0) || result;
	result = (attached_float.use_count() > 0) || result;
	result = (attached_vec.use_count() > 0) || result;
	return result;
}

float cse::EditMultiInputPanel::draw(NVGcontext* const draw_context)
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

void cse::EditMultiInputPanel::set_mouse_local_position(const Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 input_widget_offset = local_pos - Float2(0.0f, input_widget_pos);
	input_widget.set_mouse_local_position(input_widget_offset);
}

bool cse::EditMultiInputPanel::should_capture_input() const
{
	return input_widget.should_capture_input();
}

void cse::EditMultiInputPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	input_widget.handle_mouse_button(button, action, mods);
}

void cse::EditMultiInputPanel::handle_key(const int key, int scancode, const int action, const int mods)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_key(key, scancode, action, mods);
	}
}

void cse::EditMultiInputPanel::handle_character(const unsigned int codepoint)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_character(codepoint);
	}
}

void cse::EditMultiInputPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		switch (socket_value_ptr->get_type()) {
		case SocketType::INT:
		{
			const auto int_value_ptr = std::dynamic_pointer_cast<IntSocketValue>(socket_value_ptr);
			if (attached_int.lock() != int_value_ptr) {
				attached_int = int_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("Value:", attached_int);
			}
			attached_float = std::weak_ptr<FloatSocketValue>();
			attached_vec = std::weak_ptr<Float3SocketValue>();
			break;
		}
		case SocketType::FLOAT:
		{
			const auto float_value_ptr = std::dynamic_pointer_cast<FloatSocketValue>(socket_value_ptr);
			if (attached_float.lock() != float_value_ptr) {
				attached_float = float_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("Value:", attached_float);
			}
			attached_int = std::weak_ptr<IntSocketValue>();
			attached_vec = std::weak_ptr<Float3SocketValue>();
			break;
		}
		case SocketType::VECTOR:
		{
			const auto vec_value_ptr = std::dynamic_pointer_cast<Float3SocketValue>(socket_value_ptr);
			if (attached_vec.lock() != vec_value_ptr) {
				attached_vec = vec_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("X:", vec_value_ptr->x_socket_val);
				input_widget.add_socket_input("Y:", vec_value_ptr->y_socket_val);
				input_widget.add_socket_input("Z:", vec_value_ptr->z_socket_val);
			}
			attached_int = std::weak_ptr<IntSocketValue>();
			attached_float = std::weak_ptr<FloatSocketValue>();
			break;
		}
		default:
			attached_int = std::weak_ptr<IntSocketValue>();
			attached_float = std::weak_ptr<FloatSocketValue>();
			attached_vec = std::weak_ptr<Float3SocketValue>();
		}
		return;
	}
	attached_int = std::weak_ptr<IntSocketValue>();
	attached_float = std::weak_ptr<FloatSocketValue>();
	attached_vec = std::weak_ptr<Float3SocketValue>();
	input_widget.clear_sockets();
}

void cse::EditMultiInputPanel::deselect_input_box()
{
	request_undo_push = input_widget.complete_input() || request_undo_push;
}

bool cse::EditMultiInputPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = input_widget.should_push_undo_state() || result;
	return result;
}
