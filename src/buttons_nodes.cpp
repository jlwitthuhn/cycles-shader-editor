#include "buttons_nodes.h"

#include "drawing.h"
#include "gui_sizes.h"

float cse::NodeCreationButton::draw(NVGcontext* const draw_context, const Float2 draw_origin, const Float2 parent_local_mouse_pos, const float parent_width)
{
	// Coordinates to draw the actual button at
	// This is the input draw_origin adjusted to account for padding
	draw_pos = draw_origin + cse::Float2(UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	this->mouse_parent_pos = parent_local_mouse_pos;

	button_width = parent_width - 2 * UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING;
	if (button_width < 0.0f) {
		return 0.0f;
	}

	Drawing::draw_button(draw_context, draw_pos, button_width, UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT, label.c_str(), true, pressed);

	return UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT + UI_SUBWIN_NODE_LIST_BUTTON_VPADDING * 2;
}

bool cse::NodeCreationButton::is_mouse_over_button()
{
	if (mouse_parent_pos.x > draw_pos.x &&
		mouse_parent_pos.x < draw_pos.x + button_width &&
		mouse_parent_pos.y > draw_pos.y &&
		mouse_parent_pos.y < draw_pos.y + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT)
	{
		return true;
	}

	return false;
}
