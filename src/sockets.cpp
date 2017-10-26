#include "sockets.h"

CyclesShaderEditor::IntSocketValue::IntSocketValue(int default_val, int min, int max)
{
	this->default_val = default_val;
	this->min = min;
	this->max = max;

	set_value(default_val);
}

int CyclesShaderEditor::IntSocketValue::get_value()
{
	return value;
}

void CyclesShaderEditor::IntSocketValue::set_value(int value_in)
{
	if (value_in < min) {
		value = min;
	}
	else if (value_in > max) {
		value = max;
	}
	else {
		value = value_in;
	}
}

CyclesShaderEditor::FloatSocketValue::FloatSocketValue(float default_val, float min, float max)
{
	this->default_val = default_val;
	this->min = min;
	this->max = max;

	set_value(default_val);
}

float CyclesShaderEditor::FloatSocketValue::get_value()
{
	return value;
}

void CyclesShaderEditor::FloatSocketValue::set_value(float value_in)
{
	if (value_in < min) {
		value = min;
	}
	else if (value_in > max) {
		value = max;
	}
	else {
		value = value_in;
	}
}

CyclesShaderEditor::Float3SocketValue::Float3SocketValue(
	float default_x, float min_x, float max_x,
	float default_y, float min_y, float max_y,
	float default_z, float min_z, float max_z) :
	x_socket_val(default_x, min_x, max_x),
	y_socket_val(default_y, min_y, max_y),
	z_socket_val(default_z, min_z, max_z)
{

}

CyclesShaderEditor::Float3Holder CyclesShaderEditor::Float3SocketValue::get_value()
{
	Float3Holder result;
	result.x = x_socket_val.get_value();
	result.y = y_socket_val.get_value();
	result.z = z_socket_val.get_value();

	return result;
}

void CyclesShaderEditor::Float3SocketValue::set_x(float x_in)
{
	x_socket_val.set_value(x_in);
}

void CyclesShaderEditor::Float3SocketValue::set_y(float y_in)
{
	y_socket_val.set_value(y_in);
}

void CyclesShaderEditor::Float3SocketValue::set_z(float z_in)
{
	z_socket_val.set_value(z_in);
}

CyclesShaderEditor::ColorSocketValue::ColorSocketValue(float default_r, float default_g, float default_b) :
	red_socket_val(default_r, 0.0f, 1.0f),
	green_socket_val(default_g, 0.0f, 1.0f),
	blue_socket_val(default_b, 0.0f, 1.0f)
{

}

CyclesShaderEditor::StringEnumPair::StringEnumPair(std::string display_value, std::string internal_value)
{
	this->display_value = display_value;
	this->internal_value = internal_value;
}

CyclesShaderEditor::StringEnumSocketValue::StringEnumSocketValue() : value("", "")
{

}

bool CyclesShaderEditor::StringEnumSocketValue::set_from_internal_name(std::string internal_name)
{
	for (StringEnumPair this_pair : enum_values) {
		if (this_pair.internal_value == internal_name) {
			value = this_pair;
			return true;
		}
	}

	return false;
}

CyclesShaderEditor::BoolSocketValue::BoolSocketValue(bool default_val)
{
	default_value = default_val;
	value = default_value;
}

CyclesShaderEditor::FloatRGBColor CyclesShaderEditor::ColorSocketValue::get_value()
{
	FloatRGBColor value;
	value.r = red_socket_val.get_value();
	value.g = green_socket_val.get_value();
	value.b = blue_socket_val.get_value();
	return value;
}

CyclesShaderEditor::NodeSocket::NodeSocket(EditorNode* parent, SocketInOut socket_in_out, SocketType socket_type, std::string display_name, std::string internal_name)
{
	this->parent = parent;
	this->socket_in_out = socket_in_out;
	this->socket_type = socket_type;
	this->display_name = display_name;
	this->internal_name = internal_name;

	// Set selectable flag for editable data types
	if (socket_in_out == SocketInOut::Input) {
		if (socket_type == SocketType::Float || socket_type == SocketType::Color || socket_type == SocketType::StringEnum || socket_type == SocketType::Int || socket_type == SocketType::Boolean) {
			selectable = true;
		}
	}

	if (socket_type == SocketType::StringEnum || socket_type == SocketType::Int || socket_type == SocketType::Boolean) {
		draw_socket = false;
	}
}

CyclesShaderEditor::NodeSocket::~NodeSocket()
{
	if (value != nullptr) {
		delete value;
	}
}

void CyclesShaderEditor::NodeSocket::set_float_val(float float_in)
{
	if (socket_type != SocketType::Float) {
		return;
	}

	FloatSocketValue* float_val = dynamic_cast<FloatSocketValue*>(value);
	float_val->set_value(float_in);
}

void CyclesShaderEditor::NodeSocket::set_float3_val(float x_in, float y_in, float z_in)
{
	if (socket_type != SocketType::Color && socket_type != SocketType::Vector) {
		return;
	}

	if (socket_type == SocketType::Color) {
		ColorSocketValue* color_val = dynamic_cast<ColorSocketValue*>(value);
		color_val->red_socket_val.set_value(x_in);
		color_val->green_socket_val.set_value(y_in);
		color_val->blue_socket_val.set_value(z_in);
	}
	else if (socket_type == SocketType::Vector) {
		Float3SocketValue* float3_val = dynamic_cast<Float3SocketValue*>(value);
		float3_val->set_x(x_in);
		float3_val->set_y(y_in);
		float3_val->set_z(z_in);
	}
}

void CyclesShaderEditor::NodeSocket::set_string_val(StringEnumPair string_in)
{
	if (socket_type != SocketType::StringEnum) {
		return;
	}

	StringEnumSocketValue* string_enum_value = dynamic_cast<StringEnumSocketValue*>(value);
	string_enum_value->value = string_in;
}