#include "sockets.h"

#include "curve.h"

#include <algorithm>
#include <cassert>

static constexpr float CURVE_CREATE_POINT_IGNORE_MARGIN = 0.012f;
static constexpr float CURVE_POINT_SELECT_MARGIN = 0.05f;

static bool FloatPos_x_lt(CyclesShaderEditor::FloatPos a, CyclesShaderEditor::FloatPos b)
{
	return a.get_x() < b.get_x();
}

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

CyclesShaderEditor::CurveSocketValue::CurveSocketValue()
{
	reset_value();
}

void CyclesShaderEditor::CurveSocketValue::reset_value()
{
	curve_points.clear();

	FloatPos default_0(0.0f, 0.0f);
	FloatPos default_1(1.0f, 1.0f);

	curve_points.push_back(default_0);
	curve_points.push_back(default_1);
}

void CyclesShaderEditor::CurveSocketValue::create_point(float x)
{
	for (const FloatPos& this_point : curve_points) {
		const bool is_above_with_margin = x + CURVE_CREATE_POINT_IGNORE_MARGIN > this_point.get_x();
		const bool is_below_with_margin = x - CURVE_CREATE_POINT_IGNORE_MARGIN < this_point.get_x();
		if (is_above_with_margin && is_below_with_margin) {
			return;
		}
	}

	// Find the current value at x
	CurveEvaluator curve(this);
	const float y = curve.eval(x);

	curve_points.push_back(FloatPos(x, y));
	sort_curve_points();
}

void CyclesShaderEditor::CurveSocketValue::delete_point(const FloatPos& target)
{
	if (curve_points.size() <= 1) {
		return;
	}

	size_t target_index;
	bool target_found = get_target_index(target, target_index);

	if (target_found) {
		curve_points.erase(curve_points.begin() + target_index);
	}
}

bool CyclesShaderEditor::CurveSocketValue::get_target_index(const FloatPos& target, size_t& index)
{
	constexpr float MAX_DISTANCE_SQUARED = CURVE_POINT_SELECT_MARGIN * CURVE_POINT_SELECT_MARGIN;

	bool target_found = false;
	size_t target_index;
	float target_distance_squared = MAX_DISTANCE_SQUARED;
	for (size_t i = 0; i < curve_points.size(); i++) {
		const FloatPos& this_point = curve_points[i];
		const FloatPos delta = target - this_point;
		const float distance_squared = delta.get_magnitude_squared();
		if (distance_squared < target_distance_squared) {
			target_distance_squared = distance_squared;
			target_index = i;
			target_found = true;
		}
	}

	if (target_found) {
		index = target_index;
		return true;
	}

	return false;
}

size_t CyclesShaderEditor::CurveSocketValue::move_point(const size_t index, const FloatPos& new_point)
{
	if (index >= curve_points.size()) {
		// Index should always be valid
		assert(false);
	}

	curve_points[index] = new_point;
	sort_curve_points();

	for (size_t i = 0; i < curve_points.size(); i++) {
		const FloatPos this_point = curve_points[i];
		if (this_point == new_point) {
			return i;
		}
	}

	// Control should have returned already
	assert(false);
	return 0;
}

void CyclesShaderEditor::CurveSocketValue::sort_curve_points()
{
	std::sort(curve_points.begin(), curve_points.end(), FloatPos_x_lt);
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
		if (socket_type == SocketType::Float ||
			socket_type == SocketType::Color ||
			socket_type == SocketType::StringEnum ||
			socket_type == SocketType::Int ||
			socket_type == SocketType::Boolean ||
			socket_type == SocketType::Curve)
		{
			selectable = true;
		}
	}

	// Set flag to not draw
	if (socket_type == SocketType::StringEnum || socket_type == SocketType::Int || socket_type == SocketType::Boolean || socket_type == SocketType::Curve) {
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
