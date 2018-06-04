#include "click_target.h"

CyclesShaderEditor::GenericClickTarget::GenericClickTarget(const FloatPos begin_pos, const FloatPos end_pos)
{
	begin = begin_pos;
	end = end_pos;
}

bool CyclesShaderEditor::GenericClickTarget::is_mouse_over_target(const FloatPos mouse_pos) const
{
	return (mouse_pos.get_x() > begin.get_x() &&
		mouse_pos.get_x() < end.get_x() &&
		mouse_pos.get_y() > begin.get_y() &&
		mouse_pos.get_y() < end.get_y());
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::GenericClickTarget::get_normalized_mouse_pos(const CyclesShaderEditor::FloatPos mouse_pos)
{
	const float width = end.get_x() - begin.get_x();
	const float height = end.get_y() - begin.get_y();
	const float pos_x = (mouse_pos.get_x() - begin.get_x()) / width;
	const float pos_y = (mouse_pos.get_y() - begin.get_y()) / height;

	FloatPos result(pos_x, pos_y);
	result.clamp_to(FloatPos(0.0f, 0.0f), FloatPos(1.0f, 1.0f));
	return result;
}

CyclesShaderEditor::BoolValueClickTarget::BoolValueClickTarget(
	const FloatPos begin_pos,
	const FloatPos end_pos,
	const bool bool_value,
	BoolSocketValue* const socket_value
	):
	GenericClickTarget(begin_pos, end_pos)
{
	this->bool_value = bool_value;
	this->socket_value = socket_value;
}

void CyclesShaderEditor::BoolValueClickTarget::click()
{
	socket_value->value = bool_value;
}

CyclesShaderEditor::StringEnumClickTarget::StringEnumClickTarget(const FloatPos begin_pos, const FloatPos end_pos, const StringEnumPair& str_pair, StringEnumSocketValue* const enum_value) :
	GenericClickTarget(begin_pos, end_pos),
	str_pair_value(str_pair),
	enum_value(enum_value)
{

}

void CyclesShaderEditor::StringEnumClickTarget::click()
{
	enum_value->value = str_pair_value;
}

CyclesShaderEditor::CurveEditModeClickTarget::CurveEditModeClickTarget(const FloatPos begin_pos, const FloatPos end_pos, const EditCurveMode this_mode, EditCurveMode* const mode_enum) :
	GenericClickTarget(begin_pos, end_pos),
	this_mode(this_mode),
	mode_enum(mode_enum)
{

}

void CyclesShaderEditor::CurveEditModeClickTarget::click()
{
	*mode_enum = this_mode;
}

CyclesShaderEditor::CurveInterpClickTarget::CurveInterpClickTarget(const FloatPos begin_pos, const FloatPos end_pos, const CurveInterpolation this_interp, CurveInterpolation* const interp_enum) :
	GenericClickTarget(begin_pos, end_pos),
	this_interp(this_interp),
	interp_enum(interp_enum)
{

}

void CyclesShaderEditor::CurveInterpClickTarget::click()
{
	*interp_enum = this_interp;
}

CyclesShaderEditor::SocketClickTarget::SocketClickTarget(const FloatPos begin_pos, const FloatPos end_pos, NodeSocket* const socket) : GenericClickTarget(begin_pos, end_pos)
{
	this->socket = socket;
}
