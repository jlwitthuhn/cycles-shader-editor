#include "util_rectangle.h"

bool cse::do_rectangles_overlap(const Float2 low1, const Float2 high1, const Float2 low2, const Float2 high2)
{
	// Return false if there is no horizontal overlap
	if (low1.x > high2.x || low2.x > high1.x) {
		return false;
	}

	// Return false if there is no vertical overlap
	if (low1.y > high2.y || low2.y > high1.y) {
		return false;
	}

	return true;
}