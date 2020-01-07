#include "util_area.h"

#include <algorithm>

cse::Area::Area(const Float2 a, const Float2 b)
{
	const float min_x = std::min(a.x, b.x);
	const float min_y = std::min(a.y, b.y);
	const float max_x = std::max(a.x, b.x);
	const float max_y = std::max(a.y, b.y);

	begin = Float2(min_x, min_y);
	end = Float2(max_x, max_y);
}

bool cse::Area::contains_point(const Float2 pos) const
{
	return (pos.x > begin.x &&
		pos.x < end.x &&
		pos.y > begin.y &&
		pos.y < end.y);
}

cse::Float2 cse::Area::get_normalized_pos(const Float2 pos) const
{
	const Float2 size = end - begin;
	const Float2 offset = pos - begin;

	const float pos_x = offset.x / size.x;
	const float pos_y = offset.y / size.y;

	Float2 result(pos_x, pos_y);
	return result.clamp_to(Float2(0.0f, 0.0f), Float2(1.0f, 1.0f));
}
