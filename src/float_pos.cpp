#include "float_pos.h"

#include <cmath>

CyclesShaderEditor::FloatPos::FloatPos()
{
	pos_x = 0.0f;
	pos_y = 0.0f;
}

CyclesShaderEditor::FloatPos::FloatPos(float x, float y)
{
	pos_x = x;
	pos_y = y;
}

float CyclesShaderEditor::FloatPos::get_pos_x() const
{
	return pos_x;
}

float CyclesShaderEditor::FloatPos::get_pos_y() const
{
	return pos_y;
}

float CyclesShaderEditor::FloatPos::get_floor_pos_x() const
{
	return floor(pos_x);
}

float CyclesShaderEditor::FloatPos::get_floor_pos_y() const
{
	return floor(pos_y);
}

int CyclesShaderEditor::FloatPos::get_pos_x_int() const
{
	return static_cast<int>(floor(pos_x));
}

int CyclesShaderEditor::FloatPos::get_pos_y_int() const
{
	return static_cast<int>(floor(pos_y));
}

float CyclesShaderEditor::FloatPos::get_magnitude_squared() const
{
	return pos_x * pos_x + pos_y * pos_y;
}

void CyclesShaderEditor::FloatPos::clamp_to(const FloatPos& begin, const FloatPos& end)
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

CyclesShaderEditor::FloatPos CyclesShaderEditor::FloatPos::operator+(const FloatPos& other) const
{
	return FloatPos(pos_x + other.get_pos_x(), pos_y + other.get_pos_y());
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::FloatPos::operator-(const FloatPos& other) const
{
	return FloatPos(pos_x - other.get_pos_x(), pos_y - other.get_pos_y());
}

CyclesShaderEditor::FloatPos CyclesShaderEditor::FloatPos::operator/(const float& other) const
{
	return FloatPos(pos_x / other, pos_y / other);
}

bool CyclesShaderEditor::FloatPos::operator==(const FloatPos& other) const
{
	return (pos_x == other.pos_x && pos_y == other.pos_y);
}

bool CyclesShaderEditor::FloatPos::operator!=(const FloatPos& other) const
{
	return !(operator==(other));
}
