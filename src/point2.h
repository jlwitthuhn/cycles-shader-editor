#pragma once

#include <string>

namespace CyclesShaderEditor {

	class Point2 {
	public:
		Point2();
		Point2(float x, float y);

		float get_pos_x() const;
		float get_pos_y() const;

		float get_floor_pos_x() const;
		float get_floor_pos_y() const;

		int get_pos_x_int() const;
		int get_pos_y_int() const;

		float get_magnitude_squared() const;

		void clamp_to(const Point2& begin, const Point2& end);

		Point2 operator+(const Point2& other) const;
		Point2 operator-(const Point2& other) const;

		Point2 operator/(const float& other) const;

		bool operator==(const Point2& other) const;
		bool operator!=(const Point2& other) const;

	private:
		float pos_x;
		float pos_y;

		bool clamped = false;
	};

}