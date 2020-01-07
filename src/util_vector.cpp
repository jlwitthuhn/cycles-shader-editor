#include "util_vector.h"

#include <algorithm>

float cse::Float2::magnitude_squared() const
{
	return x * x + y * y;
}

cse::Float2 cse::Float2::clamp_to(const Float2& a, const Float2& b) const
{
	const float lo_x_in = std::min(a.x, b.x);
	const float lo_y_in = std::min(a.y, b.y);
	const float hi_x_in = std::max(a.x, b.x);
	const float hi_y_in = std::max(a.y, b.y);

	const float new_x = std::max(lo_x_in, std::min(hi_x_in, x));
	const float new_y = std::max(lo_y_in, std::min(hi_y_in, y));
	return Float2(new_x, new_y);
}

bool cse::Float2::is_nonzero() const
{
	return x != 0.0f || y != 0.0f;
}

cse::Float2 cse::Float2::operator+(const Float2& other) const
{
	return Float2(x + other.x, y + other.y);
}

cse::Float2 cse::Float2::operator-(const Float2& other) const
{
	return Float2(x - other.x, y - other.y);
}

void cse::Float2::operator+=(const Float2& other)
{
	x += other.x;
	y += other.y;
}

bool cse::Float2::operator==(const Float2& other) const
{
	return x == other.x && y == other.y;
}

bool cse::Float2::operator!=(const Float2& other) const
{
	return !(operator==(other));
}
