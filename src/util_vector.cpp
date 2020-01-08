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

cse::Float3 cse::Float3::rgb_as_hsv() const
{
	// Calculate Val
	const float max = std::max(x, std::max(y, z));
	const float val = max;

	// Calculate Sat
	const float min = std::min(x, std::min(y, z));
	const float range = max - min;
	if (range <= 0.0f || max <= 0.0f) {
		const float hue = -1.0f;
		const float sat = 0.0f;
		return Float3(hue, sat, val);
	}
	const float sat = range / max;

	// Calculate Hue
	auto calculate_hue = [](const Float3 rgb, const float min, const float max) -> double
	{
		constexpr double ONE_THIRD = 0.3333333333333333333333;
		constexpr double OFFSET_MULTIPLIER = ONE_THIRD / 2.0;
		const float range = max - min;

		if (rgb.x == max) {
			// Red is the top color, so the hue must be between magenta and yellow
			// offset is how far the hue is from straight red
			// It has a range of [-1, 1] with -1 meaning magenta and 1 meaning yellow
			constexpr double RED_BASE_HUE_LO = 0.0;
			constexpr double RED_BASE_HUE_HI = 1.0;
			if (rgb.y == min && rgb.z == min) {
				return 0.0f;
			}
			else {
				const double offset = (rgb.y - rgb.z) / range;
				if (offset < 0.0f) {
					return RED_BASE_HUE_HI + offset * OFFSET_MULTIPLIER;
				}
				else {
					return RED_BASE_HUE_LO + offset * OFFSET_MULTIPLIER;
				}
			}
		}
		else if (rgb.y == max) {
			constexpr double GREEN_BASE_HUE = ONE_THIRD;
			const double offset = (rgb.z - rgb.x) / range;
			return GREEN_BASE_HUE + offset * OFFSET_MULTIPLIER;
		}
		else {
			constexpr double BLUE_BASE_HUE = ONE_THIRD * 2.0;
			const double offset = (rgb.x - rgb.y) / range;
			return BLUE_BASE_HUE + offset * OFFSET_MULTIPLIER;
		}
	};

	const float hue = static_cast<float>(calculate_hue(*this, min, max));

	return Float3(hue, sat, val);
}

cse::Float3 cse::Float3::hsv_as_rgb() const
{

	const float hue = x;
	const float sat = y;
	const float val = z;

	// Special fast case for monochromatic colors
	if (sat <= 0.0) {
		return Float3(val, val, val);
	}

	const float base_hue = std::min(1.0f, std::max(0.0f, hue));
	const float scaled_hue = base_hue * 6.0f;
	const int hue_bucket = static_cast<int>(scaled_hue);
	const float hue_offset = scaled_hue - hue_bucket;
	const float p = val * (1.0f - sat);
	const float q = val * (1.0f - (sat * hue_offset));
	const float t = val * (1.0f - (sat * (1.0f - hue_offset)));

	cse::Float3 result;
	switch (hue_bucket) {
	case 0:
		result.x = val;
		result.y = t;
		result.z = p;
		break;
	case 1:
		result.x = q;
		result.y = val;
		result.z = p;
		break;
	case 2:
		result.x = p;
		result.y = val;
		result.z = t;
		break;
	case 3:
		result.x = p;
		result.y = q;
		result.z = val;
		break;
	case 4:
		result.x = t;
		result.y = p;
		result.z = val;
		break;
	case 5:
	default:
		result.x = val;
		result.y = p;
		result.z = q;
		break;
	}
	return result;
}
