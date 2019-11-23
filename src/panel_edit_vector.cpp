#include "panel_edit_vector.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"

cse::EditVectorPanel::EditVectorPanel(const float width) :
	EditParamPanel(width),
	input_widget(width)
{

}

bool cse::EditVectorPanel::is_active() const
{
	return static_cast<bool>(attached_vec.lock());
}

void cse::EditVectorPanel::pre_draw()
{

}

float cse::EditVectorPanel::draw(NVGcontext* const draw_context)
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

void cse::EditVectorPanel::set_mouse_local_position(const FloatPos local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const float input_widget_y = input_widget_pos;
	const FloatPos input_widget_pos = local_pos - FloatPos(0.0f, input_widget_y);
	input_widget.set_mouse_local_position(input_widget_pos);
}

bool cse::EditVectorPanel::should_capture_input() const
{
	bool result = false;
	result = result || input_widget.should_capture_input();
	return result;
}

void cse::EditVectorPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	input_widget.handle_mouse_button(button, action, mods);
}

void cse::EditVectorPanel::handle_key(const int key, int scancode, const int action, const int mods)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_key(key, scancode, action, mods);
	}
}

void cse::EditVectorPanel::handle_character(const unsigned int codepoint)
{
	if (input_widget.should_capture_input()) {
		input_widget.handle_character(codepoint);
	}
}

void cse::EditVectorPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::VECTOR) {
			const auto vec_value_ptr = std::dynamic_pointer_cast<Float3SocketValue>(socket_value_ptr);
			if (attached_vec.lock() != vec_value_ptr) {
				attached_vec = vec_value_ptr;
				input_widget.clear_sockets();
				input_widget.add_socket_input("X:", vec_value_ptr->x_socket_val);
				input_widget.add_socket_input("Y:", vec_value_ptr->y_socket_val);
				input_widget.add_socket_input("Z:", vec_value_ptr->z_socket_val);
			}
			return;
		}
	}
	attached_vec = std::weak_ptr<Float3SocketValue>();
	input_widget.clear_sockets();
}

void cse::EditVectorPanel::deselect_input_box()
{
	request_undo_push = request_undo_push || input_widget.complete_input();
}

bool cse::EditVectorPanel::should_push_undo_state()
{
	bool result = false;
	result = result || EditParamPanel::should_push_undo_state();
	result = result || input_widget.should_push_undo_state();
	return result;
}
