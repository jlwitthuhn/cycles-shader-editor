#pragma once

#include "output.h"

namespace cse {
	struct ColorRampPoint {
		ColorRampPoint(float position, Float3 color, float alpha);

		float position;
		Float3 color;
		float alpha;
	};
}
