#include "util_area.h"

cse::Area::Area(const FloatPos begin_pos, const FloatPos end_pos)
{
	begin = begin_pos;
	end = end_pos;
}

bool cse::Area::contains_point(const FloatPos pos) const
{
	return (pos.get_x() > begin.get_x() &&
		pos.get_x() < end.get_x() &&
		pos.get_y() > begin.get_y() &&
		pos.get_y() < end.get_y());
}

cse::FloatPos cse::Area::get_normalized_pos(const cse::FloatPos pos) const
{
	const float width = end.get_x() - begin.get_x();
	const float height = end.get_y() - begin.get_y();
	const float pos_x = (pos.get_x() - begin.get_x()) / width;
	const float pos_y = (pos.get_y() - begin.get_y()) / height;

	FloatPos result(pos_x, pos_y);
	result.clamp_to(FloatPos(0.0f, 0.0f), FloatPos(1.0f, 1.0f));
	return result;
}
