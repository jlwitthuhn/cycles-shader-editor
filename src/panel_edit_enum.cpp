#include "panel_edit_enum.h"

#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::EditEnumPanel::EditEnumPanel(const float width) :
	EditParamPanel(width),
	radio_widget(width)
{

}

bool cse::EditEnumPanel::is_active() const
{
	return attached_enum.use_count() > 0;
}

void cse::EditEnumPanel::pre_draw()
{
	radio_widget.pre_draw();
}

float cse::EditEnumPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	radio_widget_pos = height_drawn;
	height_drawn += radio_widget.draw(draw_context);

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditEnumPanel::set_mouse_local_position(const Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 radio_widget_relative = local_pos - Float2(0.0f, radio_widget_pos);
	radio_widget.set_mouse_local_position(radio_widget_relative);
}

void cse::EditEnumPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	radio_widget.handle_mouse_button(button, action, mods);
}

void cse::EditEnumPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::STRING_ENUM) {
			const auto enum_value_ptr = std::dynamic_pointer_cast<StringEnumSocketValue>(socket_value_ptr);
			if (attached_enum.lock() != enum_value_ptr) {
				attached_enum = enum_value_ptr;
				radio_widget.attach_value(attached_enum);
			}
			return;
		}
	}
	attached_enum = std::weak_ptr<StringEnumSocketValue>();
	radio_widget.attach_value(attached_enum);
}

bool cse::EditEnumPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = radio_widget.should_push_undo_state() || result;
	return result;
}
