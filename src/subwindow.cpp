#include "subwindow.h"

#include <nanovg.h>

#include "gui_sizes.h"

cse::NodeEditorSubwindow::NodeEditorSubwindow(FloatPos screen_position, std::string title)
{
	subwindow_screen_pos = screen_position;
	this->title = title;
}

cse::FloatPos cse::NodeEditorSubwindow::get_screen_pos() const
{
	return subwindow_screen_pos;
}

float cse::NodeEditorSubwindow::get_width() const
{
	return subwindow_width;
}

float cse::NodeEditorSubwindow::get_height() const
{
	return content_height + UI_SUBWIN_HEADER_HEIGHT + 3.0f;
}

void cse::NodeEditorSubwindow::pre_draw()
{
	// Do nothing by default
}

void cse::NodeEditorSubwindow::draw(NVGcontext* draw_context)
{
	if (is_active() == false) {
		return;
	}

	// Draw window
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, get_width(), get_height(), UI_SUBWIN_CORNER_RADIUS);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Header
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, UI_SUBWIN_HEADER_HEIGHT, UI_SUBWIN_CORNER_RADIUS);
	nvgRect(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT / 2, subwindow_width, UI_SUBWIN_HEADER_HEIGHT / 2);
	if (is_mouse_over_header()) {
		nvgFillColor(draw_context, nvgRGBA(225, 225, 225, 255));
	}
	else {
		nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
	}
	nvgFill(draw_context);

	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgStrokeWidth(draw_context, 0.8f);

	nvgBeginPath(draw_context);
	nvgMoveTo(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT);
	nvgLineTo(draw_context, subwindow_width, UI_SUBWIN_HEADER_HEIGHT);
	nvgStroke(draw_context);

	// Outline
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, get_height(), UI_SUBWIN_CORNER_RADIUS);
	nvgStroke(draw_context);

	// Title
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, subwindow_width / 2, UI_SUBWIN_HEADER_HEIGHT / 2, title.c_str(), nullptr);

	// Set up context and call draw_content
	nvgSave(draw_context);
	nvgTranslate(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT + 3.0f);
	nvgScissor(draw_context, 0.0f, 0.0f, get_width(), content_height);
	draw_content(draw_context);
	nvgRestore(draw_context);
}

bool cse::NodeEditorSubwindow::is_mouse_over() const
{
	if (subwindow_moving) {
		return true;
	}
	if (is_active() == false) {
		return false;
	}
	return (
		mouse_local_pos.get_x() > 0.0f &&
		mouse_local_pos.get_y() > 0.0f &&
		mouse_local_pos.get_x() < get_width() &&
		mouse_local_pos.get_y() < get_height()
		);
}

bool cse::NodeEditorSubwindow::is_mouse_over_header() const
{
	if (subwindow_moving) {
		return true;
	}
	if (is_active() == false) {
		return false;
	}
	return (mouse_local_pos.get_x() > 0 &&
		mouse_local_pos.get_x() < subwindow_width &&
		mouse_local_pos.get_y() > 0 &&
		mouse_local_pos.get_y() < UI_SUBWIN_HEADER_HEIGHT);
}

void cse::NodeEditorSubwindow::set_mouse_position(FloatPos local_position, float max_safe_pos_y)
{
	if (subwindow_moving) {
		subwindow_screen_pos = subwindow_screen_pos + (local_position - mouse_local_begin_move_pos);
		if (subwindow_screen_pos.get_y() + UI_SUBWIN_HEADER_HEIGHT > max_safe_pos_y) {
			subwindow_screen_pos = FloatPos(subwindow_screen_pos.get_x(), max_safe_pos_y - UI_SUBWIN_HEADER_HEIGHT);
		}
		mouse_local_pos = mouse_local_begin_move_pos;
	}
	else {
		mouse_local_pos = local_position;
	}
	mouse_content_pos = mouse_local_pos - FloatPos(0.0f, UI_SUBWIN_HEADER_HEIGHT + 3.0f);
}

bool cse::NodeEditorSubwindow::should_capture_input() const
{
	return false;
}

void cse::NodeEditorSubwindow::deselect_input_box()
{

}

void cse::NodeEditorSubwindow::handle_mouse_button(int /*button*/, int /*action*/, int /*mods*/)
{

}

void cse::NodeEditorSubwindow::handle_key(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{

}

void cse::NodeEditorSubwindow::handle_character(unsigned int /*codepoint*/)
{

}

void cse::NodeEditorSubwindow::move_window_begin()
{
	subwindow_moving = true;
	mouse_local_begin_move_pos = mouse_local_pos;
}

void cse::NodeEditorSubwindow::move_window_end()
{
	subwindow_moving = false;
}

bool cse::NodeEditorSubwindow::needs_undo_push()
{
	return false;
}

void cse::NodeEditorSubwindow::update_selection(std::weak_ptr<const Selection> /*selection*/)
{
	// Do nothing
}