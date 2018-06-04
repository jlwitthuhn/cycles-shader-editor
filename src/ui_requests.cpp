#include "ui_requests.h"

CyclesShaderEditor::UIRequests::UIRequests()
{
	clear();
}

void CyclesShaderEditor::UIRequests::clear()
{
	save = false;
	undo = false;
	redo = false;

	move_left = false;
	move_right = false;
	move_up = false;
	move_down = false;

	zoom_in = false;
	zoom_out = false;
}

void CyclesShaderEditor::UIRequests::operator|=(const UIRequests& other)
{
	save = save || other.save;
	undo = undo || other.undo;
	redo = redo || other.redo;

	move_left = move_left || other.move_left;
	move_right = move_right || other.move_right;
	move_up = move_up || other.move_up;
	move_down = move_down || other.move_down;

	zoom_in = zoom_in || other.zoom_in;
	zoom_out = zoom_out || other.zoom_out;
}
