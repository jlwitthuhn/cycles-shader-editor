#pragma once

namespace cse {

	class Float2 {
	public:
		Float2() : x(0.0f), y(0.0f) {}
		Float2(const float x, const float y) : x(x), y(y) {}

		float magnitude_squared() const;

		Float2 clamp_to(const Float2& a, const Float2& b) const;

		bool is_nonzero() const;

		Float2 operator+(const Float2& other) const;
		Float2 operator-(const Float2& other) const;

		void operator+=(const Float2& other);

		bool operator==(const Float2& other) const;
		bool operator!=(const Float2& other) const;

		float x;
		float y;
	};

	class Float3 {
	public:
		Float3() : x(0.0f), y(0.0f), z(0.0f) {}
		Float3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

		float x;
		float y;
		float z;
	};
}
