#include "sockets.h"

#include "curve.h"

#include <algorithm>
#include <cassert>
#include <memory>

static constexpr float CURVE_CREATE_POINT_IGNORE_MARGIN = 0.012f;
static constexpr float CURVE_POINT_SELECT_MARGIN = 0.05f;

static bool FloatPos_x_lt(cse::FloatPos a, cse::FloatPos b)
{
	return a.get_x() < b.get_x();
}

cse::IntSocketValue::IntSocketValue(int default_val, int min, int max)
{
	this->default_val = default_val;
	this->min = min;
	this->max = max;

	set_value(default_val);
}

cse::SocketType cse::IntSocketValue::get_type() const
{
	return SocketType::INT;
}

int cse::IntSocketValue::get_value()
{
	return value;
}

void cse::IntSocketValue::set_value(int value_in)
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

cse::FloatSocketValue::FloatSocketValue(float default_val, float min, float max)
{
	this->default_val = default_val;
	this->min = min;
	this->max = max;

	set_value(default_val);
}

cse::SocketType cse::FloatSocketValue::get_type() const
{
	return SocketType::FLOAT;
}

float cse::FloatSocketValue::get_value()
{
	return value;
}

void cse::FloatSocketValue::set_value(float value_in)
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

cse::Float3SocketValue::Float3SocketValue(
	float default_x, float min_x, float max_x,
	float default_y, float min_y, float max_y,
	float default_z, float min_z, float max_z) :
	x_socket_val(std::make_shared<FloatSocketValue>(default_x, min_x, max_x)),
	y_socket_val(std::make_shared<FloatSocketValue>(default_y, min_y, max_y)),
	z_socket_val(std::make_shared<FloatSocketValue>(default_z, min_z, max_z))
{

}

cse::SocketType cse::Float3SocketValue::get_type() const
{
	return SocketType::VECTOR;
}

cse::Float3Holder cse::Float3SocketValue::get_value()
{
	Float3Holder result;
	result.x = x_socket_val->get_value();
	result.y = y_socket_val->get_value();
	result.z = z_socket_val->get_value();

	return result;
}

void cse::Float3SocketValue::set_x(float x_in)
{
	x_socket_val->set_value(x_in);
}

void cse::Float3SocketValue::set_y(float y_in)
{
	y_socket_val->set_value(y_in);
}

void cse::Float3SocketValue::set_z(float z_in)
{
	z_socket_val->set_value(z_in);
}

cse::ColorSocketValue::ColorSocketValue(float default_r, float default_g, float default_b) :
	red_socket_val(std::make_shared<FloatSocketValue>(default_r, 0.0f, 1.0f)),
	green_socket_val(std::make_shared<FloatSocketValue>(default_g, 0.0f, 1.0f)),
	blue_socket_val(std::make_shared<FloatSocketValue>(default_b, 0.0f, 1.0f))
{

}

cse::SocketType cse::ColorSocketValue::get_type() const
{
	return SocketType::COLOR;
}

cse::StringEnumPair::StringEnumPair(std::string display_value, std::string internal_value)
{
	this->display_value = display_value;
	this->internal_value = internal_value;
}

cse::StringEnumSocketValue::StringEnumSocketValue() : value("", "")
{

}

cse::SocketType cse::StringEnumSocketValue::get_type() const
{
	return SocketType::STRING_ENUM;
}

bool cse::StringEnumSocketValue::set_from_internal_name(std::string internal_name)
{
	for (StringEnumPair this_pair : enum_values) {
		if (this_pair.internal_value == internal_name) {
			value = this_pair;
			return true;
		}
	}

	return false;
}

cse::BoolSocketValue::BoolSocketValue(bool default_val)
{
	default_value = default_val;
	value = default_value;
}

cse::SocketType cse::BoolSocketValue::get_type() const
{
	return SocketType::BOOLEAN;
}

cse::CurveSocketValue::CurveSocketValue()
{
	reset_value();
}

cse::SocketType cse::CurveSocketValue::get_type() const
{
	return SocketType::CURVE;
}

void cse::CurveSocketValue::reset_value()
{
	curve_points.clear();

	FloatPos default_0(0.0f, 0.0f);
	FloatPos default_1(1.0f, 1.0f);

	curve_points.push_back(default_0);
	curve_points.push_back(default_1);
}

void cse::CurveSocketValue::create_point(float x)
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

void cse::CurveSocketValue::delete_point(const FloatPos& target)
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

bool cse::CurveSocketValue::get_target_index(const FloatPos& target, std::size_t& index)
{
	constexpr float MAX_DISTANCE_SQUARED = CURVE_POINT_SELECT_MARGIN * CURVE_POINT_SELECT_MARGIN;

	bool target_found = false;
	size_t target_index = 0;
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

size_t cse::CurveSocketValue::move_point(const std::size_t index, const FloatPos& new_point)
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

void cse::CurveSocketValue::sort_curve_points()
{
	std::sort(curve_points.begin(), curve_points.end(), FloatPos_x_lt);
}

cse::FloatRGBColor cse::ColorSocketValue::get_value()
{
	FloatRGBColor value;
	value.r = red_socket_val->get_value();
	value.g = green_socket_val->get_value();
	value.b = blue_socket_val->get_value();
	return value;
}

cse::NodeSocket::NodeSocket(EditorNode* parent, SocketIOType io_type, SocketType socket_type, std::string display_name, std::string internal_name)
{
	this->parent = parent;
	this->io_type = io_type;
	this->socket_type = socket_type;
	this->display_name = display_name;
	this->internal_name = internal_name;

	// Set selectable flag for editable data types
	if (io_type == SocketIOType::INPUT) {
		if (socket_type == SocketType::FLOAT ||
			socket_type == SocketType::COLOR ||
			socket_type == SocketType::STRING_ENUM ||
			socket_type == SocketType::INT ||
			socket_type == SocketType::BOOLEAN ||
			socket_type == SocketType::CURVE)
		{
			selectable = true;
		}
	}

	// Set flag to not draw
	if (socket_type == SocketType::STRING_ENUM || socket_type == SocketType::INT || socket_type == SocketType::BOOLEAN || socket_type == SocketType::CURVE) {
		draw_socket = false;
	}
}

void cse::NodeSocket::set_float_val(float float_in)
{
	if (socket_type != SocketType::FLOAT) {
		return;
	}

	const std::shared_ptr<FloatSocketValue> float_val = std::dynamic_pointer_cast<FloatSocketValue>(value);
	if (float_val) {
		float_val->set_value(float_in);
	}
}

void cse::NodeSocket::set_float3_val(float x_in, float y_in, float z_in)
{
	if (socket_type != SocketType::COLOR && socket_type != SocketType::VECTOR) {
		return;
	}

	if (socket_type == SocketType::COLOR) {
		const std::shared_ptr<ColorSocketValue> color_val = std::dynamic_pointer_cast<ColorSocketValue>(value);
		if (color_val) {
			color_val->red_socket_val->set_value(x_in);
			color_val->green_socket_val->set_value(y_in);
			color_val->blue_socket_val->set_value(z_in);
		}
	}
	else if (socket_type == SocketType::VECTOR) {
		const std::shared_ptr<Float3SocketValue> float3_val = std::dynamic_pointer_cast<Float3SocketValue>(value);
		if (float3_val) {
			float3_val->set_x(x_in);
			float3_val->set_y(y_in);
			float3_val->set_z(z_in);
		}
	}
}

void cse::NodeSocket::set_string_val(StringEnumPair string_in)
{
	if (socket_type != SocketType::STRING_ENUM) {
		return;
	}

	const std::shared_ptr<StringEnumSocketValue> string_enum_val = std::dynamic_pointer_cast<StringEnumSocketValue>(value);
	if (string_enum_val) {
		string_enum_val->value = string_in;
	}
}
