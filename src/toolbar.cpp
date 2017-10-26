#include "toolbar.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include <iostream>

#include "gui_sizes.h"
#include "ui_requests.h"

CyclesShaderEditor::ToolbarButton::ToolbarButton(ToolbarButtonType type)
{
	this->type = type;
}

CyclesShaderEditor::NodeEditorToolbar::NodeEditorToolbar(UIRequests* requests)
{
	this->requests = requests;

	buttons.push_back(ToolbarButton(ToolbarButtonType::SAVE));
	buttons.push_back(ToolbarButton(ToolbarButtonType::SPACER));
	buttons.push_back(ToolbarButton(ToolbarButtonType::UNDO));
	buttons.push_back(ToolbarButton(ToolbarButtonType::REDO));
	buttons.push_back(ToolbarButton(ToolbarButtonType::SPACER));
	buttons.push_back(ToolbarButton(ToolbarButtonType::ZOOM_IN));
	buttons.push_back(ToolbarButton(ToolbarButtonType::ZOOM_OUT));
}

void CyclesShaderEditor::NodeEditorToolbar::draw(NVGcontext* draw_context, float toolbar_width)
{
	nvgSave(draw_context);

	// Draw panel
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, toolbar_width, get_toolbar_height(), 0.0f);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Draw buttons
	float section_begin_x = 0;
	for (ToolbarButton this_button: buttons) {
		if (this_button.type == ToolbarButtonType::SPACER) {

			const float X_POS = section_begin_x + UI_TOOLBAR_SPACER_WIDTH / 2.0f;

			nvgBeginPath(draw_context);
			nvgStrokeWidth(draw_context, 0.75f);
			nvgStrokeColor(draw_context, nvgRGBf(0.1f, 0.1f, 0.1f));

			nvgMoveTo(draw_context, X_POS, UI_TOOLBAR_SPACER_VPAD);
			nvgLineTo(draw_context, X_POS, get_toolbar_height() - UI_TOOLBAR_SPACER_VPAD);

			nvgStroke(draw_context);

			section_begin_x += UI_TOOLBAR_SPACER_WIDTH;
			continue;
		}

		const int GRADIENT_ALPHA = this_button.enabled ? 31 : 15;
		const int BUTTON_ALPHA = this_button.enabled ? 127 : 63;
		const int TEXT_ALPHA = this_button.enabled ? 255 : 127;

		// Calculate things we need for the button
		Point2 button_origin(section_begin_x + UI_TOOLBAR_BUTTON_HPAD, UI_TOOLBAR_BUTTON_VPAD);
		const float button_height = get_toolbar_height() - 2 * UI_TOOLBAR_BUTTON_VPAD;
		NVGpaint button_bg;
		if (this_button.pressed) {
			button_bg = nvgLinearGradient(draw_context,
				button_origin.get_pos_x(),
				button_origin.get_pos_y(),
				button_origin.get_pos_x(),
				button_origin.get_pos_y() + button_height,
				nvgRGBA(0, 0, 0, GRADIENT_ALPHA),
				nvgRGBA(255, 255, 255, GRADIENT_ALPHA));
		}
		else {
			button_bg = nvgLinearGradient(draw_context,
				button_origin.get_pos_x(),
				button_origin.get_pos_y(),
				button_origin.get_pos_x(),
				button_origin.get_pos_y() + button_height,
				nvgRGBA(255, 255, 255, GRADIENT_ALPHA),
				nvgRGBA(0, 0, 0, GRADIENT_ALPHA));
		}

		// Draw button
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context,
			button_origin.get_pos_x(),
			button_origin.get_pos_y(),
			UI_TOOLBAR_BUTTON_WIDTH,
			button_height,
			UI_BUTTON_CORNER_RADIUS);
		nvgFillPaint(draw_context, button_bg);
		nvgFill(draw_context);
		nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, BUTTON_ALPHA));
		nvgStrokeWidth(draw_context, 1.0f);
		nvgStroke(draw_context);

		// Button label
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgFontFace(draw_context, "sans");
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, TEXT_ALPHA));
		if (this_button.type == ToolbarButtonType::SAVE) {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Save", NULL);
		}
		else if (this_button.type == ToolbarButtonType::UNDO) {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Undo", NULL);
		}
		else if (this_button.type == ToolbarButtonType::REDO) {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Redo", NULL);
		}
		else if (this_button.type == ToolbarButtonType::ZOOM_IN) {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Zoom In", NULL);
		}
		else if (this_button.type == ToolbarButtonType::ZOOM_OUT) {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Zoom Out", NULL);
		}
		else {
			nvgText(draw_context, button_origin.get_pos_x() + UI_TOOLBAR_BUTTON_WIDTH / 2, button_origin.get_pos_y() + button_height / 2, "Error", NULL);
		}

		section_begin_x += UI_TOOLBAR_BUTTON_WIDTH + UI_TOOLBAR_BUTTON_HPAD * 2;
	}

	nvgRestore(draw_context);
}

bool CyclesShaderEditor::NodeEditorToolbar::is_mouse_over()
{
	return (
		mouse_screen_pos.get_pos_y() > 0.0f &&
		mouse_screen_pos.get_pos_y() < get_toolbar_height()
		);
}

void CyclesShaderEditor::NodeEditorToolbar::release_buttons()
{
	for (ToolbarButton& this_button : buttons) {
		this_button.pressed = false;
	}
}

void CyclesShaderEditor::NodeEditorToolbar::disable_button(ToolbarButtonType type)
{
	for (ToolbarButton& this_button : buttons) {
		if (this_button.type == type) {
			this_button.enabled = false;
			return;
		}
	}
}

void CyclesShaderEditor::NodeEditorToolbar::enable_button(ToolbarButtonType type)
{
	for (ToolbarButton& this_button : buttons) {
		if (this_button.type == type) {
			this_button.enabled = true;
			return;
		}
	}
}

void CyclesShaderEditor::NodeEditorToolbar::set_mouse_position(Point2 screen_position)
{
	mouse_screen_pos = screen_position;
}

void CyclesShaderEditor::NodeEditorToolbar::handle_mouse_button(int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		ToolbarButton* button = get_button_under_mouse();
		if (button != nullptr) {
			button->pressed = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		ToolbarButton* button = get_button_under_mouse();
		if (button != nullptr) {
			if (button->pressed) {
				set_request(button->type);
			}
			button->pressed = false;
		}
	}
}

CyclesShaderEditor::ToolbarButton* CyclesShaderEditor::NodeEditorToolbar::get_button_under_mouse()
{
	const float BUTTON_MIN_Y = UI_TOOLBAR_BUTTON_VPAD;
	const float BUTTON_MAX_Y = get_toolbar_height() - UI_TOOLBAR_BUTTON_VPAD;

	float section_begin_x = 0;
	for (ToolbarButton& this_button : buttons) {
		if (this_button.type == ToolbarButtonType::SPACER) {
			section_begin_x += UI_TOOLBAR_SPACER_WIDTH;
			continue;
		}
		const float BUTTON_MIN_X = section_begin_x + UI_TOOLBAR_BUTTON_HPAD;
		const float BUTTON_MAX_X = BUTTON_MIN_X + UI_TOOLBAR_BUTTON_WIDTH;

		if (mouse_screen_pos.get_pos_y() > BUTTON_MIN_Y &&
			mouse_screen_pos.get_pos_y() < BUTTON_MAX_Y &&
			mouse_screen_pos.get_pos_x() > BUTTON_MIN_X &&
			mouse_screen_pos.get_pos_x() < BUTTON_MAX_X)
		{
			return &this_button;
		}

		section_begin_x += UI_TOOLBAR_BUTTON_WIDTH + UI_TOOLBAR_BUTTON_HPAD * 2;
	}

	return nullptr;
}

void CyclesShaderEditor::NodeEditorToolbar::set_request(ToolbarButtonType button_type)
{
	switch (button_type) {
	case ToolbarButtonType::SAVE:
		if (requests->save == false) {
			requests->save = true;
		}
		break;
	case ToolbarButtonType::UNDO:
		if (requests->undo == false) {
			requests->undo = true;
		}
		break;
	case ToolbarButtonType::REDO:
		if (requests->redo == false) {
			requests->redo = true;
		}
		break;
	case ToolbarButtonType::ZOOM_IN:
		if (requests->zoom_in == false) {
			requests->zoom_in = true;
		}
		break;
	case ToolbarButtonType::ZOOM_OUT:
		if (requests->zoom_out == false) {
			requests->zoom_out = true;
		}
		break;
	default:
		break;
	}
}