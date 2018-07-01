#include "panel_edit.h"

CyclesShaderEditor::ParamEditorPanel::ParamEditorPanel(const float width) :
	panel_width(width)
{

}

void CyclesShaderEditor::ParamEditorPanel::pre_draw()
{
	// Stub
}

void CyclesShaderEditor::ParamEditorPanel::set_mouse_local_position(const FloatPos local_pos)
{
	mouse_local_pos = local_pos;
}

bool CyclesShaderEditor::ParamEditorPanel::is_mouse_over()
{
	if (is_active() == false) {
		return false;
	}

	const float min_x = 0.0f;
	const float max_x = panel_width;
	const float min_y = 0.0f;
	const float max_y = panel_height;

	if (mouse_local_pos.get_x() > min_x &&
		mouse_local_pos.get_x() < max_x &&
		mouse_local_pos.get_y() > min_y &&
		mouse_local_pos.get_y() < max_y)
	{
		return true;
	}

	return false;
}

bool CyclesShaderEditor::ParamEditorPanel::should_capture_input() const
{
	return false;
}

void CyclesShaderEditor::ParamEditorPanel::handle_mouse_button(int, int, int)
{
	// Stub
}

void CyclesShaderEditor::ParamEditorPanel::handle_key(int, int, int, int)
{
	// Stub
}

void CyclesShaderEditor::ParamEditorPanel::handle_character(unsigned int)
{
	// Stub
}

void CyclesShaderEditor::ParamEditorPanel::deselect_input_box()
{
	// Stub
}

bool CyclesShaderEditor::ParamEditorPanel::should_push_undo_state()
{
	return false;
}

void CyclesShaderEditor::ParamEditorPanel::reset()
{
	// Stub
}
