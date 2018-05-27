#include "buttons_nodes.h"

#include "drawing.h"
#include "gui_sizes.h"
#include "node_colors.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"

float CyclesShaderEditor::NodeCreationButton::draw(NVGcontext* const draw_context, const FloatPos draw_origin, const FloatPos parent_local_mouse_pos, const float parent_width)
{
	// Coordinates to draw the actual button at
	// This is the input draw_origin adjusted to account for padding
	draw_pos = draw_origin + CyclesShaderEditor::FloatPos(UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	this->mouse_parent_pos = parent_local_mouse_pos;

	button_width = parent_width - 2 * UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING;
	if (button_width < 0.0f) {
		return 0.0f;
	}

	Drawing::draw_button(draw_context, draw_pos, button_width, UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT, label.c_str(), true, pressed);

	return UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT + UI_SUBWIN_NODE_LIST_BUTTON_VPADDING * 2;
}

bool CyclesShaderEditor::NodeCreationButton::is_mouse_over_button()
{
	if (mouse_parent_pos.get_x() > draw_pos.get_x() &&
		mouse_parent_pos.get_x() < draw_pos.get_x() + button_width &&
		mouse_parent_pos.get_y() > draw_pos.get_y() &&
		mouse_parent_pos.get_y() < draw_pos.get_y() + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT)
	{
		return true;
	}

	return false;
}

CyclesShaderEditor::EditorNode* CyclesShaderEditor::NodeCreationButton::create_node()
{
	const FloatPos default_pos(0.0f, 0.0f);
	return create_node(default_pos);
}
