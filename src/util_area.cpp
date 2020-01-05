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

cse::CurveEditModeArea::CurveEditModeArea(const FloatPos begin_pos, const FloatPos end_pos, const EditCurveMode this_mode, EditCurveMode* const mode_enum) :
	Area(begin_pos, end_pos),
	this_mode(this_mode),
	mode_enum(mode_enum)
{

}

bool cse::CurveEditModeArea::click()
{
	if (*mode_enum != this_mode) {
		*mode_enum = this_mode;
		return true;
	}
	return false;
}

cse::CurveInterpModeArea::CurveInterpModeArea(const FloatPos begin_pos, const FloatPos end_pos, const CurveInterpolation this_interp, CurveInterpolation* const interp_enum) :
	Area(begin_pos, end_pos),
	this_interp(this_interp),
	interp_enum(interp_enum)
{

}

bool cse::CurveInterpModeArea::click()
{
	if (*interp_enum != this_interp) {
		*interp_enum = this_interp;
		return true;
	}
	return false;
}

cse::SocketArea::SocketArea(const FloatPos begin_pos, const FloatPos end_pos, const std::weak_ptr<NodeSocket> socket) :
	Area(begin_pos, end_pos),
	socket(socket)
{

}
