#include "util_color.h"

// RGB/HSV conversion algorithms adapted from https://stackoverflow.com/questions/3018313

CyclesShaderEditor::HueSatVal CyclesShaderEditor::hsv_from_rgb(RedGreenBlue rgb)
{
	CyclesShaderEditor::HueSatVal hsv;

	// Calculate Val
	float max = rgb.r;
	if (rgb.g > max) {
		max = rgb.g;
	}
	if (rgb.b > max) {
		max = rgb.b;
	}
	hsv.val = max;

	// Calculate Sat
	float min = rgb.r;
	if (rgb.g < min) {
		min = rgb.g;
	}
	if (rgb.b < min) {
		min = rgb.b;
	}
	
	const float range = max - min;

	if (range <= 0.0f || max <= 0.0f) {
		// Monochrome image
		hsv.sat = 0;
		hsv.hue = -1.0f;
		return hsv;
	}
	hsv.sat = range / max;

	// Calculate Hue
	constexpr float ONE_THIRD = 0.3333333333333333333333f;
	constexpr float OFFSET_MULTIPLIER = ONE_THIRD / 2.0f;
	if (rgb.r == max) {
		// Red is the top color, so the hue must be between magenta and yellow
		// offset is how far the hue is from straight red
		// It has a range of [-1, 1] with -1 meaning magenta and 1 meaning yellow
		constexpr float RED_BASE_HUE = 0.0f;
		const float offset = (rgb.g - rgb.b) / range;
		hsv.hue = RED_BASE_HUE + offset * OFFSET_MULTIPLIER;
	}
	else {
		if (rgb.g == max) {
			constexpr float GREEN_BASE_HUE = ONE_THIRD;
			const float offset = (rgb.b - rgb.r) / range;
			hsv.hue = GREEN_BASE_HUE + offset * OFFSET_MULTIPLIER;
		}
		else {
			constexpr float BLUE_BASE_HUE = ONE_THIRD * 2.0f;
			const float offset = (rgb.r - rgb.g) / range;
			hsv.hue = BLUE_BASE_HUE + offset * OFFSET_MULTIPLIER;
		}
	}

	return hsv;
}

CyclesShaderEditor::RedGreenBlue CyclesShaderEditor::rgb_from_hsv(HueSatVal hsv)
{
	CyclesShaderEditor::RedGreenBlue rgb;

	// Special fast case for monochromatic colors
	if (hsv.sat <= 0.0) {
		rgb.r = hsv.val;
		rgb.g = hsv.val;
		rgb.b = hsv.val;
		return rgb;
	}

	const float base_hue = hsv.hue >= 1.0f ? 0.0f : hsv.hue;
	const float scaled_hue = base_hue * 6.0f;
	const int hue_bucket = static_cast<int>(scaled_hue);
	const float hue_offset = scaled_hue - hue_bucket;
	const float p = hsv.val * (1.0f - hsv.sat);
	const float q = hsv.val * (1.0f - (hsv.sat * hue_offset));
	const float t = hsv.val * (1.0f - (hsv.sat * (1.0f - hue_offset)));

	switch (hue_bucket) {
	case 0:
		rgb.r = hsv.val;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = hsv.val;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = hsv.val;
		rgb.b = t;
		break;
	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = hsv.val;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = hsv.val;
		break;
	case 5:
	default:
		rgb.r = hsv.val;
		rgb.g = p;
		rgb.b = q;
		break;
	}
	return rgb;
}
