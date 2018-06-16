#include "ui_requests.h"

CyclesShaderEditor::ViewUIRequests::ViewUIRequests()
{
	clear();
}

void CyclesShaderEditor::ViewUIRequests::clear()
{
	pan_left = false;
	pan_right = false;
	pan_up = false;
	pan_down = false;

	zoom_in = false;
	zoom_out = false;
}

void CyclesShaderEditor::ViewUIRequests::operator|=(const ViewUIRequests& other)
{
	pan_left = pan_left || other.pan_left;
	pan_right = pan_right || other.pan_right;
	pan_up = pan_up || other.pan_up;
	pan_down = pan_down || other.pan_down;

	zoom_in = zoom_in || other.zoom_in;
	zoom_out = zoom_out || other.zoom_out;
}

CyclesShaderEditor::UIRequests::UIRequests()
{
	clear();
}

void CyclesShaderEditor::UIRequests::clear()
{
	save = false;
	undo = false;
	redo = false;

	view.clear();
}

void CyclesShaderEditor::UIRequests::operator|=(const UIRequests& other)
{
	save = save || other.save;
	undo = undo || other.undo;
	redo = redo || other.redo;

	view |= other.view;
}
