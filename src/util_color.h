#pragma once

namespace cse {

	class RedGreenBlue {
	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};

	class HueSatVal {
	public:
		float hue = 0.0f;
		float sat = 0.0f;
		float val = 0.0f;
	};

	// Returns the given RGB color as HSV
	// In the case of monochrome images, hue will be negative
	HueSatVal hsv_from_rgb(RedGreenBlue rgb);

	// Returns the given HSV color as RGB
	RedGreenBlue rgb_from_hsv(HueSatVal in);
}
