#pragma once

#include "util_vector.h"

namespace cse {

	class Area {
	public:
		Area(Float2 a, Float2 b);

		bool contains_point(Float2 pos) const;
		Float2 get_normalized_pos(Float2 pos) const;

	protected:
		Float2 begin;
		Float2 end;
	};

	template<typename T>
	class HolderArea : public Area {
	public:
		HolderArea(const Float2 begin_pos, const Float2 end_pos, T hold_value) :
			Area(begin_pos, end_pos),
			value(hold_value)
			{}

		T get_value() const { return value; }

	protected:
		T value;
	};
}
