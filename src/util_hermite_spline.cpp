#include "util_hermite_spline.h"

// Cubic hermit spline interpolation
// Code taken from https://blog.demofox.org/2015/08/08/cubic-hermite-interpolation/

CyclesShaderEditor::CubicHermiteSplineInterpolator::CubicHermiteSplineInterpolator(float A, float B, float C, float D)
{
	a = -A / 2.0f + (3.0f*B) / 2.0f - (3.0f*C) / 2.0f + D / 2.0f;
	b = A - (5.0f*B) / 2.0f + 2.0f*C - D / 2.0f;
	c = -A / 2.0f + C / 2.0f;
	d = B;
}

float CyclesShaderEditor::CubicHermiteSplineInterpolator::eval(float t) const
{
	return a*t*t*t + b*t*t + c*t + d;
}
