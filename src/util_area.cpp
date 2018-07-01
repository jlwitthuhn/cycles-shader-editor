#include "util_area.h"

CyclesShaderEditor::Area::Area(const FloatPos begin_pos, const FloatPos end_pos)
{
	begin = begin_pos;
	end = end_pos;
}

bool CyclesShaderEditor::Area::is_under_point(const FloatPos pos) const
{
	return (pos.get_x() > begin.get_x() &&
		pos.get_x() < end.get_x() &&
		pos.get_y() > begin.get_y() &&
		pos.get_y() < end.get_y());
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::Area::get_normalized_mouse_pos(const CyclesShaderEditor::FloatPos mouse_pos)
{
	const float width = end.get_x() - begin.get_x();
	const float height = end.get_y() - begin.get_y();
	const float pos_x = (mouse_pos.get_x() - begin.get_x()) / width;
	const float pos_y = (mouse_pos.get_y() - begin.get_y()) / height;

	FloatPos result(pos_x, pos_y);
	result.clamp_to(FloatPos(0.0f, 0.0f), FloatPos(1.0f, 1.0f));
	return result;
}

CyclesShaderEditor::BoolValueArea::BoolValueArea(
	const FloatPos begin_pos,
	const FloatPos end_pos,
	const bool bool_value,
	const std::weak_ptr<BoolSocketValue> socket_value
	):
	Area(begin_pos, end_pos),
	bool_value(bool_value),
	socket_value(socket_value)
{

}

void CyclesShaderEditor::BoolValueArea::click()
{
	if (const auto socket_value_ptr = socket_value.lock()) {
		socket_value_ptr->value = bool_value;
	}
}

CyclesShaderEditor::StringEnumArea::StringEnumArea(const FloatPos begin_pos, const FloatPos end_pos, const StringEnumPair& str_pair, const std::weak_ptr<StringEnumSocketValue> enum_value) :
	Area(begin_pos, end_pos),
	str_pair_value(str_pair),
	enum_value(enum_value)
{

}

void CyclesShaderEditor::StringEnumArea::click()
{
	if (const auto enum_value_ptr = enum_value.lock()) {
		enum_value_ptr->value = str_pair_value;
	}
}

CyclesShaderEditor::CurveEditModeArea::CurveEditModeArea(const FloatPos begin_pos, const FloatPos end_pos, const EditCurveMode this_mode, EditCurveMode* const mode_enum) :
	Area(begin_pos, end_pos),
	this_mode(this_mode),
	mode_enum(mode_enum)
{

}

void CyclesShaderEditor::CurveEditModeArea::click()
{
	*mode_enum = this_mode;
}

CyclesShaderEditor::CurveInterpModeArea::CurveInterpModeArea(const FloatPos begin_pos, const FloatPos end_pos, const CurveInterpolation this_interp, CurveInterpolation* const interp_enum) :
	Area(begin_pos, end_pos),
	this_interp(this_interp),
	interp_enum(interp_enum)
{

}

void CyclesShaderEditor::CurveInterpModeArea::click()
{
	*interp_enum = this_interp;
}

CyclesShaderEditor::SocketArea::SocketArea(const FloatPos begin_pos, const FloatPos end_pos, const std::weak_ptr<NodeSocket> socket) :
	Area(begin_pos, end_pos),
	socket(socket)
{

}
