#include "point2.h"

#include <cmath>

CyclesShaderEditor::Point2::Point2()
{
	pos_x = 0.0f;
	pos_y = 0.0f;
}

CyclesShaderEditor::Point2::Point2(float x, float y)
{
	pos_x = x;
	pos_y = y;
}

float CyclesShaderEditor::Point2::get_pos_x() const
{
	return pos_x;
}

float CyclesShaderEditor::Point2::get_pos_y() const
{
	return pos_y;
}

float CyclesShaderEditor::Point2::get_floor_pos_x() const
{
	return floor(pos_x);
}

float CyclesShaderEditor::Point2::get_floor_pos_y() const
{
	return floor(pos_y);
}

int CyclesShaderEditor::Point2::get_pos_x_int() const
{
	return static_cast<int>(floor(pos_x));
}

int CyclesShaderEditor::Point2::get_pos_y_int() const
{
	return static_cast<int>(floor(pos_y));
}

float CyclesShaderEditor::Point2::get_magnitude_squared() const
{
	return pos_x * pos_x + pos_y * pos_y;
}

void CyclesShaderEditor::Point2::clamp_to(const Point2& begin, const Point2& end)
{
	if (pos_x < begin.get_pos_x()) {
		pos_x = begin.pos_x;
		clamped = true;
	}
	else if (pos_x > end.get_pos_x()) {
		pos_x = end.get_pos_x();
		clamped = true;
	}
	if (pos_y < begin.get_pos_y()) {
		pos_y = begin.pos_y;
		clamped = true;
	}
	else if (pos_y > end.get_pos_y()) {
		pos_y = end.get_pos_y();
		clamped = true;
	}
}

CyclesShaderEditor::Point2 CyclesShaderEditor::Point2::operator+(const Point2& other) const
{
	return Point2(pos_x + other.get_pos_x(), pos_y + other.get_pos_y());
}

CyclesShaderEditor::Point2 CyclesShaderEditor::Point2::operator-(const Point2& other) const
{
	return Point2(pos_x - other.get_pos_x(), pos_y - other.get_pos_y());
}

CyclesShaderEditor::Point2 CyclesShaderEditor::Point2::operator/(const float& other) const
{
	return Point2(pos_x / other, pos_y / other);
}

bool CyclesShaderEditor::Point2::operator==(const Point2& other) const
{
	return (pos_x == other.pos_x && pos_y == other.pos_y);
}
