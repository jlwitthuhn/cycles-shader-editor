#include "sockets.h"

#include "curve.h"
#include "output.h"

#include <algorithm>
#include <cassert>
#include <memory>

static constexpr float CURVE_CREATE_POINT_IGNORE_MARGIN = 0.012f;
static constexpr float CURVE_POINT_SELECT_MARGIN = 0.05f;

static bool Float2_x_lt(const cse::Float2 a, const cse::Float2 b)
{
	return a.x < b.x;
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

cse::Float3 cse::Float3SocketValue::get_value()
{
	const float x = x_socket_val->get_value();
	const float y = y_socket_val->get_value();
	const float z = z_socket_val->get_value();
	const Float3 result(x, y, z);
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
	r_socket_val(std::make_shared<FloatSocketValue>(default_r, 0.0f, 1.0f)),
	g_socket_val(std::make_shared<FloatSocketValue>(default_g, 0.0f, 1.0f)),
	b_socket_val(std::make_shared<FloatSocketValue>(default_b, 0.0f, 1.0f))
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

	Float2 default_0(0.0f, 0.0f);
	Float2 default_1(1.0f, 1.0f);

	curve_points.push_back(default_0);
	curve_points.push_back(default_1);
}

void cse::CurveSocketValue::create_point(float x)
{
	for (const Float2& this_point : curve_points) {
		const bool is_above_with_margin = x + CURVE_CREATE_POINT_IGNORE_MARGIN > this_point.x;
		const bool is_below_with_margin = x - CURVE_CREATE_POINT_IGNORE_MARGIN < this_point.x;
		if (is_above_with_margin && is_below_with_margin) {
			return;
		}
	}

	// Find the current value at x
	CurveEvaluator curve(this);
	const float y = curve.eval(x);

	curve_points.push_back(Float2(x, y));
	sort_curve_points();
}

void cse::CurveSocketValue::delete_point(const Float2& target)
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

bool cse::CurveSocketValue::get_target_index(const Float2& target, std::size_t& index)
{
	constexpr float MAX_DISTANCE_SQUARED = CURVE_POINT_SELECT_MARGIN * CURVE_POINT_SELECT_MARGIN;

	bool target_found = false;
	size_t target_index = 0;
	float target_distance_squared = MAX_DISTANCE_SQUARED;
	for (size_t i = 0; i < curve_points.size(); i++) {
		const Float2& this_point = curve_points[i];
		const Float2 delta = target - this_point;
		const float distance_squared = delta.magnitude_squared();
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

size_t cse::CurveSocketValue::move_point(const std::size_t index, const Float2& new_point)
{
	if (index >= curve_points.size()) {
		// Index should always be valid
		assert(false);
	}

	curve_points[index] = new_point;
	sort_curve_points();

	for (size_t i = 0; i < curve_points.size(); i++) {
		const Float2 this_point = curve_points[i];
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
	std::sort(curve_points.begin(), curve_points.end(), Float2_x_lt);
}

cse::ColorRampSocketValue::ColorRampSocketValue()
{
	ColorRampPoint p1(0.0f, Float3(0.0f, 0.0f, 0.0f), 1.0f);
	ColorRampPoint p2(1.0f, Float3(1.0f, 1.0f, 1.0f), 1.0f);
	ramp_points.push_back(p1);
	ramp_points.push_back(p2);
}

cse::SocketType cse::ColorRampSocketValue::get_type() const
{
	return SocketType::COLOR_RAMP;
}

std::vector<cse::Float4> cse::ColorRampSocketValue::evaluate_samples() const
{
	constexpr size_t SAMPLE_COUNT = 256;
	constexpr float POS_BEGIN = 0.0f;
	constexpr float POS_END = 1.0f;
	constexpr float POS_INCREMENT = (POS_END - POS_BEGIN) / (SAMPLE_COUNT - 1);

	std::vector<Float4> result;

	// Create a function-local copy of the control point vector so we can be sure it is sorted
	std::vector<ColorRampPoint> ramp_points = this->ramp_points;
	std::sort(
		ramp_points.begin(),
		ramp_points.end(),
		[](const ColorRampPoint& lhs, const ColorRampPoint& rhs) {
			return lhs.position < rhs.position;
		}
	);

	if (ramp_points.size() == 0) {
		// Invalid ramp, interpret as all black
		result.push_back(Float4(0.0f, 0.0f, 0.0f, 1.0f));
		result.push_back(Float4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	const float pos_first_change = ramp_points[0].position;
	const Float3 color_start = ramp_points[0].color;
	const float alpha_start = ramp_points[0].alpha;

	const float pos_last_change = ramp_points[ramp_points.size() - 1].position;
	const Float3 color_end = ramp_points[ramp_points.size() - 1].color;
	const float alpha_end = ramp_points[ramp_points.size() - 1].alpha;

	{
		size_t index_search_begin = 0;
		for (size_t i = 0; i < SAMPLE_COUNT; i++) {
			const float this_pos = POS_BEGIN + i * POS_INCREMENT;
			if (this_pos <= pos_first_change) {
				Float4 this_output(color_start.x, color_start.y, color_start.z, alpha_start);
				result.push_back(this_output);
			}
			else if (this_pos >= pos_last_change) {
				Float4 this_output(color_end.x, color_end.y, color_end.z, alpha_end);
				result.push_back(this_output);
			}
			else {
				while (ramp_points[index_search_begin + 1].position <= this_pos) {
					index_search_begin++;
				}

				const ColorRampPoint low = ramp_points[index_search_begin];
				const ColorRampPoint high = ramp_points[index_search_begin + 1];

				const float segment_size = high.position - low.position;
				const float relative_pos = this_pos - low.position;
				const float lerp_val = relative_pos / segment_size;

				const float out_r = low.color.x + (high.color.x - low.color.x) * lerp_val;
				const float out_g = low.color.y + (high.color.y - low.color.y) * lerp_val;
				const float out_b = low.color.z + (high.color.z - low.color.z) * lerp_val;
				const float out_a = low.alpha + (high.alpha - low.alpha) * lerp_val;

				result.push_back(Float4(out_r, out_g, out_b, out_a));
			}
		}
	}


	return result;
}

cse::Float3 cse::ColorSocketValue::get_value()
{
	const float r = r_socket_val->get_value();
	const float g = g_socket_val->get_value();
	const float b = b_socket_val->get_value();
	const Float3 result(r, g, b);
	return result;
}

cse::NodeSocket::NodeSocket(EditableNode* parent, SocketIOType io_type, SocketType socket_type, std::string display_name, std::string internal_name) :
	parent(parent)
{
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
			socket_type == SocketType::CURVE ||
			socket_type == SocketType::COLOR_RAMP)
		{
			selectable = true;
		}
	}

	// Set flag to not draw
	if (socket_type == SocketType::STRING_ENUM ||
		socket_type == SocketType::INT ||
		socket_type == SocketType::BOOLEAN ||
		socket_type == SocketType::CURVE ||
		socket_type == SocketType::COLOR_RAMP)
	{
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
			color_val->r_socket_val->set_value(x_in);
			color_val->g_socket_val->set_value(y_in);
			color_val->b_socket_val->set_value(z_in);
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
