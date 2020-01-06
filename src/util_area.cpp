#include "util_area.h"

cse::Area::Area(const Float2 begin_pos, const Float2 end_pos)
{
	begin = begin_pos;
	end = end_pos;
}

bool cse::Area::contains_point(const Float2 pos) const
{
	return (pos.x > begin.x &&
		pos.x < end.x &&
		pos.y > begin.y &&
		pos.y < end.y);
}

cse::Float2 cse::Area::get_normalized_pos(const cse::Float2 pos) const
{
	const float width = end.x - begin.x;
	const float height = end.y - begin.y;
	const float pos_x = (pos.x - begin.x) / width;
	const float pos_y = (pos.y - begin.y) / height;

	Float2 result(pos_x, pos_y);
	return result.clamp_to(Float2(0.0f, 0.0f), Float2(1.0f, 1.0f));
}
