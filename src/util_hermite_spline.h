#pragma once

namespace cse {
	class CubicHermiteSplineInterpolator {
	public:
		CubicHermiteSplineInterpolator(float A, float B, float C, float D);

		float eval(float t) const;

	private:
		const float a;
		const float b;
		const float c;
		const float d;
	};
}
