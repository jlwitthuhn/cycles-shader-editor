#include "util_area.h"

#include <algorithm>

cse::Area::Area(const Float2 a, const Float2 b)
{
	const float min_x = std::min(a.x, b.x);
	const float min_y = std::min(a.y, b.y);
	const float max_x = std::max(a.x, b.x);
	const float max_y = std::max(a.y, b.y);

	lo = Float2(min_x, min_y);
	hi = Float2(max_x, max_y);
}

bool cse::Area::contains_point(const Float2 pos) const
{
	return (pos.x > lo.x &&
		pos.x < hi.x &&
		pos.y > lo.y &&
		pos.y < hi.y);
}

bool cse::Area::overlaps(const Area other) const
{
	// Return false if there is no horizontal overlap
	if (lo.x > other.hi.x || other.lo.x > hi.x) {
		return false;
	}

	// Return false if there is no vertical overlap
	if (lo.y > other.hi.y || other.lo.y > hi.y) {
		return false;
	}

	return true;
}

cse::Float2 cse::Area::get_normalized_pos(const Float2 pos) const
{
	const Float2 size = hi - lo;
	const Float2 offset = pos - lo;

	const float pos_x = offset.x / size.x;
	const float pos_y = offset.y / size.y;

	Float2 result(pos_x, pos_y);
	return result.clamp_to(Float2(0.0f, 0.0f), Float2(1.0f, 1.0f));
}
