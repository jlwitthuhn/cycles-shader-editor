#include "toolbar.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "gui_sizes.h"
#include "ui_requests.h"

cse::ToolbarButton::ToolbarButton(const ToolbarButtonType type)
{
	this->type = type;
}

std::string cse::ToolbarButton::get_label() const
{
	switch (type) {
	case ToolbarButtonType::SAVE:
		return "Save";
	case ToolbarButtonType::UNDO:
		return "Undo";
	case ToolbarButtonType::REDO:
		return "Redo";
	case ToolbarButtonType::ZOOM_IN:
		return "Zoom In";
	case ToolbarButtonType::ZOOM_OUT:
		return "Zoom Out";
	default:
		return "Error";
	}
}

void cse::ToolbarButton::set_geometry(const FloatPos pos_in, const float width_in, const float height_in)
{
	pos = pos_in;
	width = width_in;
	height = height_in;
}

bool cse::ToolbarButton::contains_point(const FloatPos point) const
{
	const float min_x = pos.get_x();
	const float max_x = pos.get_x() + width;
	const float min_y = pos.get_y();
	const float max_y = pos.get_y() + height;
	return (
		point.get_x() >= min_x &&
		point.get_x() <= max_x &&
		point.get_y() >= min_y &&
		point.get_y() <= max_y
	);
}

cse::NodeEditorToolbar::NodeEditorToolbar()
{
	buttons.push_back(ToolbarButton(ToolbarButtonType::SAVE));
	buttons.push_back(ToolbarButton(ToolbarButtonType::SPACER));
	buttons.push_back(ToolbarButton(ToolbarButtonType::UNDO));
	buttons.push_back(ToolbarButton(ToolbarButtonType::REDO));
	buttons.push_back(ToolbarButton(ToolbarButtonType::SPACER));
	buttons.push_back(ToolbarButton(ToolbarButtonType::ZOOM_IN));
	buttons.push_back(ToolbarButton(ToolbarButtonType::ZOOM_OUT));
}

void cse::NodeEditorToolbar::draw(NVGcontext* const draw_context, const float toolbar_width)
{
	nvgSave(draw_context);

	// Draw panel
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, toolbar_width, get_toolbar_height(), 0.0f);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Draw buttons
	float section_begin_x = 0;
	for (ToolbarButton& this_button: buttons) {
		if (this_button.type == ToolbarButtonType::SPACER) {

			const float x_pos = section_begin_x + UI_TOOLBAR_SPACER_WIDTH / 2.0f;

			nvgBeginPath(draw_context);
			nvgStrokeWidth(draw_context, 0.75f);
			nvgStrokeColor(draw_context, nvgRGBf(0.1f, 0.1f, 0.1f));

			nvgMoveTo(draw_context, x_pos, UI_TOOLBAR_SPACER_VPAD);
			nvgLineTo(draw_context, x_pos, get_toolbar_height() - UI_TOOLBAR_SPACER_VPAD);

			nvgStroke(draw_context);

			section_begin_x += UI_TOOLBAR_SPACER_WIDTH;
			continue;
		}

		const FloatPos button_pos(section_begin_x + UI_TOOLBAR_BUTTON_HPAD, UI_TOOLBAR_BUTTON_VPAD);
		const float button_height = get_toolbar_height() - 2 * UI_TOOLBAR_BUTTON_VPAD;
		Drawing::draw_button(draw_context, button_pos, UI_TOOLBAR_BUTTON_WIDTH, button_height, this_button.get_label(), this_button.enabled, this_button.pressed);
		this_button.set_geometry(button_pos, UI_TOOLBAR_BUTTON_WIDTH, button_height);

		section_begin_x += UI_TOOLBAR_BUTTON_WIDTH + UI_TOOLBAR_BUTTON_HPAD * 2;
	}

	nvgRestore(draw_context);
}

void cse::NodeEditorToolbar::set_button_enabled(const ToolbarButtonType type, const bool enabled)
{
	for (ToolbarButton& this_button : buttons) {
		if (this_button.type == type) {
			this_button.enabled = enabled;
			return;
		}
	}
}

bool cse::NodeEditorToolbar::is_mouse_over()
{
	return (
		mouse_screen_pos.get_y() > 0.0f &&
		mouse_screen_pos.get_y() < get_toolbar_height()
		);
}

void cse::NodeEditorToolbar::set_mouse_position(FloatPos screen_position)
{
	mouse_screen_pos = screen_position;
}

void cse::NodeEditorToolbar::handle_mouse_button(int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		press_button_under_mouse();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		release_button_under_mouse();
		for (ToolbarButton& this_button : buttons) {
			this_button.pressed = false;
		}
	}
}

cse::UIRequests cse::NodeEditorToolbar::consume_ui_requests()
{
	const UIRequests result = requests;
	requests.clear();
	return result;
}

void cse::NodeEditorToolbar::press_button_under_mouse()
{
	for (ToolbarButton& button : buttons) {
		if (button.contains_point(mouse_screen_pos)) {
			button.pressed = true;
		}
	}
}

void cse::NodeEditorToolbar::release_button_under_mouse()
{
	for (ToolbarButton& button : buttons) {
		if (button.contains_point(mouse_screen_pos)) {
			if (button.pressed) {
				set_request(button.type);
			}
			button.pressed = false;
		}
	}
}

void cse::NodeEditorToolbar::set_request(ToolbarButtonType button_type)
{
	switch (button_type) {
	case ToolbarButtonType::SAVE:
		requests.save = true;
		break;
	case ToolbarButtonType::UNDO:
		requests.undo = true;
		break;
	case ToolbarButtonType::REDO:
		requests.redo = true;
		break;
	case ToolbarButtonType::ZOOM_IN:
		requests.view.zoom_in = true;
		break;
	case ToolbarButtonType::ZOOM_OUT:
		requests.view.zoom_out = true;
		break;
	default:
		break;
	}
}
