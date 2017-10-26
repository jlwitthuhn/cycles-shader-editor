#include "click_target.h"

CyclesShaderEditor::GenericClickTarget::GenericClickTarget(Point2 begin_pos, Point2 end_pos)
{
	begin = begin_pos;
	end = end_pos;
}

bool CyclesShaderEditor::GenericClickTarget::is_mouse_over_target(Point2 mouse_pos)
{
	return (mouse_pos.get_pos_x() > begin.get_pos_x() &&
		mouse_pos.get_pos_x() < end.get_pos_x() &&
		mouse_pos.get_pos_y() > begin.get_pos_y() &&
		mouse_pos.get_pos_y() < end.get_pos_y());
}

CyclesShaderEditor::BoolValueClickTarget::BoolValueClickTarget(Point2 begin_pos, Point2 end_pos, bool bool_value, BoolSocketValue* socket_value) : GenericClickTarget(begin_pos, end_pos)
{
	this->bool_value = bool_value;
	this->socket_value = socket_value;
}

void CyclesShaderEditor::BoolValueClickTarget::click()
{
	socket_value->value = bool_value;
}

CyclesShaderEditor::StringEnumClickTarget::StringEnumClickTarget(Point2 begin_pos, Point2 end_pos, StringEnumPair str_pair, StringEnumSocketValue* enum_value) :
	GenericClickTarget(begin_pos, end_pos),
	str_pair_value(str_pair.display_value, str_pair.internal_value)
{
	this->enum_value = enum_value;
}

void CyclesShaderEditor::StringEnumClickTarget::click()
{
	enum_value->value = str_pair_value;
}

CyclesShaderEditor::SocketClickTarget::SocketClickTarget(Point2 begin_pos, Point2 end_pos, NodeSocket* socket) : GenericClickTarget(begin_pos, end_pos)
{
	this->socket = socket;
}