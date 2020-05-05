#include "panel_edit.h"

cse::EditParamPanel::EditParamPanel(const float width) :
	panel_width(width)
{

}

float cse::EditParamPanel::get_width() const
{
	return panel_width;
}

float cse::EditParamPanel::get_height() const
{
	return panel_height;
}

void cse::EditParamPanel::pre_draw()
{
	// Stub so subclasses can optionally override
}

void cse::EditParamPanel::set_mouse_local_position(const Float2 local_pos)
{
	mouse_local_pos = local_pos;
}

bool cse::EditParamPanel::is_mouse_over() const
{
	if (is_active() == false) {
		return false;
	}

	const float min_x = 0.0f;
	const float max_x = panel_width;
	const float min_y = 0.0f;
	const float max_y = panel_height;

	if (mouse_local_pos.x > min_x &&
		mouse_local_pos.x < max_x &&
		mouse_local_pos.y > min_y &&
		mouse_local_pos.y < max_y)
	{
		return true;
	}

	return false;
}

bool cse::EditParamPanel::has_input_focus() const
{
	return false;
}

void cse::EditParamPanel::handle_mouse_button(int, int, int)
{
	// Stub
}

void cse::EditParamPanel::handle_key(int, int, int, int)
{
	// Stub
}

void cse::EditParamPanel::handle_character(unsigned int)
{
	// Stub
}

void cse::EditParamPanel::deselect_input_box()
{
	// Stub
}


bool cse::EditParamPanel::should_push_undo_state()
{
	bool result = request_undo_push;
	request_undo_push = false;
	return result;
}

void cse::EditParamPanel::tab()
{
	// Stub
}

void cse::EditParamPanel::reset()
{
	// Stub
}
