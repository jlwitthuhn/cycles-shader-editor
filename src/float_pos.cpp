#include "float_pos.h"

#include <cmath>

cse::FloatPos::FloatPos()
{
	pos_x = 0.0f;
	pos_y = 0.0f;
}

cse::FloatPos::FloatPos(float x, float y)
{
	pos_x = x;
	pos_y = y;
}

float cse::FloatPos::get_x() const
{
	return pos_x;
}

float cse::FloatPos::get_y() const
{
	return pos_y;
}

float cse::FloatPos::get_floor_x() const
{
	return floor(pos_x);
}

float cse::FloatPos::get_floor_y() const
{
	return floor(pos_y);
}

float cse::FloatPos::get_round_x() const
{
	return roundf(pos_x);
}

float cse::FloatPos::get_round_y() const
{
	return roundf(pos_y);
}

int cse::FloatPos::get_x_as_int() const
{
	return static_cast<int>(floor(pos_x));
}

int cse::FloatPos::get_y_as_int() const
{
	return static_cast<int>(floor(pos_y));
}

float cse::FloatPos::get_magnitude_squared() const
{
	return pos_x * pos_x + pos_y * pos_y;
}

bool cse::FloatPos::is_nonzero() const
{
	return (pos_x != 0.0f || pos_y != 0.0f);
}

void cse::FloatPos::clamp_to(const FloatPos& begin, const FloatPos& end)
{
	if (pos_x < begin.get_x()) {
		pos_x = begin.pos_x;
		clamped = true;
	}
	else if (pos_x > end.get_x()) {
		pos_x = end.get_x();
		clamped = true;
	}
	if (pos_y < begin.get_y()) {
		pos_y = begin.pos_y;
		clamped = true;
	}
	else if (pos_y > end.get_y()) {
		pos_y = end.get_y();
		clamped = true;
	}
}

cse::FloatPos cse::FloatPos::operator+(const FloatPos& other) const
{
	return FloatPos(pos_x + other.get_x(), pos_y + other.get_y());
}

cse::FloatPos cse::FloatPos::operator-(const FloatPos& other) const
{
	return FloatPos(pos_x - other.get_x(), pos_y - other.get_y());
}

void cse::FloatPos::operator+=(const FloatPos& other)
{
	pos_x += other.pos_x;
	pos_y += other.pos_y;
}

cse::FloatPos cse::FloatPos::operator*(const float& other) const
{
	return FloatPos(pos_x * other, pos_y * other);
}

cse::FloatPos cse::FloatPos::operator/(const float& other) const
{
	return FloatPos(pos_x / other, pos_y / other);
}

bool cse::FloatPos::operator==(const FloatPos& other) const
{
	return (pos_x == other.pos_x && pos_y == other.pos_y);
}

bool cse::FloatPos::operator!=(const FloatPos& other) const
{
	return !(operator==(other));
}
