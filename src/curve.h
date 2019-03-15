#pragma once

#include <memory>
#include <vector>

#include "float_pos.h"

namespace cse {
	class CurveSocketValue;

	// This class is used to store the output of some curve in a format that allows for quick evaluation
	// It will linearly interpolate between sampled values
	// The curve may not have multiple points with equal X values
	class CurveEvaluator {
	public:
		CurveEvaluator(CurveSocketValue* curve_socket_val, int segments = 512);
		CurveEvaluator(std::shared_ptr<CurveSocketValue> curve_socket_val, int segments = 512);
		CurveEvaluator(FloatPos a, FloatPos b, FloatPos c, FloatPos d, int segments = 128);

		int compare_to_range(float in_value) const;
		float eval(float in_value) const;

	private:
		std::vector<FloatPos> sampled_points;
	};
}
