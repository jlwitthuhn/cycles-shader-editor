#include "subwindow.h"

#include "gui_sizes.h"

CyclesShaderEditor::NodeEditorSubwindow::NodeEditorSubwindow(Point2 screen_position)
{
	subwindow_screen_pos = screen_position;
}

CyclesShaderEditor::Point2 CyclesShaderEditor::NodeEditorSubwindow::get_screen_pos() const
{
	return subwindow_screen_pos;
}

bool CyclesShaderEditor::NodeEditorSubwindow::is_mouse_over()
{
	if (subwindow_moving) {
		return true;
	}

	if (is_subwindow_active() == false) {
		return false;
	}

	return (
		mouse_local_pos.get_pos_x() > 0.0f &&
		mouse_local_pos.get_pos_y() > 0.0f &&
		mouse_local_pos.get_pos_x() < subwindow_width &&
		mouse_local_pos.get_pos_y() < subwindow_height );
}

void CyclesShaderEditor::NodeEditorSubwindow::set_mouse_position(Point2 local_position, float max_safe_pos_y)
{
	if (subwindow_moving) {
		subwindow_screen_pos = subwindow_screen_pos + (local_position - mouse_local_begin_move_pos);
		if (subwindow_screen_pos.get_pos_y() + UI_SUBWIN_HEADER_HEIGHT > max_safe_pos_y) {
			subwindow_screen_pos = Point2(subwindow_screen_pos.get_pos_x(), max_safe_pos_y - UI_SUBWIN_HEADER_HEIGHT);
		}
		mouse_local_pos = mouse_local_begin_move_pos;
	}
	else {
		mouse_local_pos = local_position;
	}

}

void CyclesShaderEditor::NodeEditorSubwindow::handle_mouse_button(int /*button*/, int /*action*/, int /*mods*/)
{

}

void CyclesShaderEditor::NodeEditorSubwindow::handle_key(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{

}

void CyclesShaderEditor::NodeEditorSubwindow::handle_character(unsigned int /*codepoint*/)
{

}

void CyclesShaderEditor::NodeEditorSubwindow::move_window_begin()
{
	subwindow_moving = true;
	mouse_local_begin_move_pos = mouse_local_pos;
}

void CyclesShaderEditor::NodeEditorSubwindow::move_window_end()
{
	subwindow_moving = false;
}