#include "util_rectangle.h"

bool CyclesShaderEditor::do_rectangles_overlap(Point2 low1, Point2 high1, Point2 low2, Point2 high2)
{
	// Return false if there is no horizontal overlap
	if (low1.get_pos_x() > high2.get_pos_x() || low2.get_pos_x() > high1.get_pos_x()) {
		return false;
	}

	// Return false if there is no vertical overlap
	if (low1.get_pos_y() > high2.get_pos_y() || low2.get_pos_y() > high1.get_pos_y()) {
		return false;
	}

	return true;
}