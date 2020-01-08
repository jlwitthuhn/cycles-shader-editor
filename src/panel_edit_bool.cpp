#include "panel_edit_bool.h"

#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::EditBoolPanel::EditBoolPanel(const float width) :
	EditParamPanel(width),
	radio_widget(width)
{

}

bool cse::EditBoolPanel::is_active() const
{
	return attached_bool.use_count() > 0;
}

void cse::EditBoolPanel::pre_draw()
{
	radio_widget.pre_draw();
}

float cse::EditBoolPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	radio_widget_pos = height_drawn;
	height_drawn += radio_widget.draw(draw_context);

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditBoolPanel::set_mouse_local_position(const Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 radio_widget_relative = local_pos - Float2(0.0f, radio_widget_pos);
	radio_widget.set_mouse_local_position(radio_widget_relative);
}

void cse::EditBoolPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	radio_widget.handle_mouse_button(button, action, mods);
}

void cse::EditBoolPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::BOOLEAN) {
			const auto bool_value_ptr = std::dynamic_pointer_cast<BoolSocketValue>(socket_value_ptr);
			if (attached_bool.lock() != bool_value_ptr) {
				attached_bool = bool_value_ptr;
				radio_widget.attach_value(attached_bool);
			}
			return;
		}
	}
	attached_bool = std::weak_ptr<BoolSocketValue>();
	radio_widget.attach_value(attached_bool);
}

bool cse::EditBoolPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = radio_widget.should_push_undo_state() || result;
	return result;
}
