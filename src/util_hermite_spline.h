#pragma once

namespace CyclesShaderEditor {
	class CubicHermiteSplineInterpolator {
	public:
		CubicHermiteSplineInterpolator(float A, float B, float C, float D);

		float eval(float t) const;

	private:
		float a;
		float b;
		float c;
		float d;
	};
}
