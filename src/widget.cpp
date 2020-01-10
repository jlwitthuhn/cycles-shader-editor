#include "widget.h"

cse::Widget::Widget(const float width) : width(width)
{

}

cse::Widget::~Widget()
{

}

float cse::Widget::draw(NVGcontext*)
{
	return 0.0f;
}

void cse::Widget::set_mouse_local_pos(const Float2 local_pos)
{
	mouse_pos = local_pos;
}

bool cse::Widget::has_input_focus() const
{
	return false;
}

void cse::Widget::handle_mouse_button(int, int, int)
{

}

void cse::Widget::handle_key(int, int, int, int)
{

}

void cse::Widget::handle_character(unsigned int)
{

}

bool cse::Widget::should_push_undo_state()
{
	const bool result = request_undo_push;
	request_undo_push = false;
	return result;
}
