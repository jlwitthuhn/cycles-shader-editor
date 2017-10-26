#include "buttons_nodes.h"

#include <nanovg.h>

#include "gui_sizes.h"
#include "node_colors.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"

float CyclesShaderEditor::NodeCreationButton::draw_button(NVGcontext* draw_context, Point2 draw_origin, Point2 parent_local_mouse_pos, float content_width)
{
	// Coordinates to draw the actual button at
	// This is the input draw_origin adjusted to account for margin
	draw_pos = draw_origin + CyclesShaderEditor::Point2(UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	this->mouse_parent_pos = parent_local_mouse_pos;

	button_width = content_width - 2 * UI_SUBWIN_NODE_LIST_NODE_BUTTON_HPADDING;
	if (button_width < 0.0f) {
		return 0.0f;
	}

	nvgSave(draw_context);

	NVGpaint button_bg;
	if (pressed) {
		button_bg = nvgLinearGradient(draw_context,
			draw_pos.get_pos_x(),
			draw_pos.get_pos_y(),
			draw_pos.get_pos_x(),
			draw_pos.get_pos_y() + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT,
			nvgRGBA(0, 0, 0, 32),
			nvgRGBA(255, 255, 255, 32));
	}
	else {
		button_bg = nvgLinearGradient(draw_context,
			draw_pos.get_pos_x(),
			draw_pos.get_pos_y(),
			draw_pos.get_pos_x(),
			draw_pos.get_pos_y() + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT,
			nvgRGBA(255, 255, 255, 32),
			nvgRGBA(0, 0, 0, 32));
	}

	// Button shape
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context,
		draw_pos.get_pos_x(),
		draw_pos.get_pos_y(),
		button_width,
		UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT,
		UI_BUTTON_CORNER_RADIUS);
	nvgFillPaint(draw_context, button_bg);
	nvgFill(draw_context);
	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 127));
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStroke(draw_context);

	// Button label
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, draw_pos.get_pos_x() + button_width / 2, draw_pos.get_pos_y() + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT / 2, label.c_str(), NULL);

	nvgRestore(draw_context);

	return UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT + UI_SUBWIN_NODE_LIST_BUTTON_VPADDING * 2;
}

bool CyclesShaderEditor::NodeCreationButton::is_mouse_over_button()
{
	if (mouse_parent_pos.get_pos_x() > draw_pos.get_pos_x() &&
		mouse_parent_pos.get_pos_x() < draw_pos.get_pos_x() + button_width &&
		mouse_parent_pos.get_pos_y() > draw_pos.get_pos_y() &&
		mouse_parent_pos.get_pos_y() < draw_pos.get_pos_y() + UI_SUBWIN_NODE_LIST_NODE_BUTTON_HEIGHT)
	{
		return true;
	}

	return false;
}
