#include "util_rectangle.h"

bool CyclesShaderEditor::do_rectangles_overlap(FloatPos low1, FloatPos high1, FloatPos low2, FloatPos high2)
{
	// Return false if there is no horizontal overlap
	if (low1.get_x() > high2.get_x() || low2.get_x() > high1.get_x()) {
		return false;
	}

	// Return false if there is no vertical overlap
	if (low1.get_y() > high2.get_y() || low2.get_y() > high1.get_y()) {
		return false;
	}

	return true;
}