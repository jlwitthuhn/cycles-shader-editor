#pragma once

#include <string>

namespace CyclesShaderEditor {

	// Represents some position in 2d space
	class FloatPos {
	public:
		FloatPos();
		FloatPos(float x, float y);

		float get_x() const;
		float get_y() const;

		float get_floor_x() const;
		float get_floor_y() const;

		int get_x_as_int() const;
		int get_y_as_int() const;

		float get_magnitude_squared() const;

		bool is_nonzero() const;

		void clamp_to(const FloatPos& begin, const FloatPos& end);

		FloatPos operator+(const FloatPos& other) const;
		FloatPos operator-(const FloatPos& other) const;
		void operator+=(const FloatPos& other);

		FloatPos operator/(const float& other) const;

		bool operator==(const FloatPos& other) const;
		bool operator!=(const FloatPos& other) const;

	private:
		float pos_x;
		float pos_y;

		bool clamped = false;
	};

}
