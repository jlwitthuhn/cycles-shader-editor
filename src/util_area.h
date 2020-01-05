#pragma once

#include <memory>

#include "common_enums.h"
#include "float_pos.h"
#include "sockets.h"

namespace cse {

	class Area {
	public:
		Area(FloatPos begin_pos, FloatPos end_pos);

		bool contains_point(FloatPos pos) const;
		FloatPos get_normalized_pos(FloatPos pos) const;

	protected:
		FloatPos begin;
		FloatPos end;
	};

	template<typename T>
	class HolderArea : public Area {
	public:
		HolderArea(const FloatPos begin_pos, const FloatPos end_pos, T hold_value) :
			Area(begin_pos, end_pos),
			value(hold_value)
			{}

		T get_value() const { return value; }

	protected:
		T value;
	};

	class StringEnumArea : public Area {
	public:
		StringEnumArea(FloatPos begin_pos, FloatPos end_pos, const StringEnumPair& str_pair, std::weak_ptr<StringEnumSocketValue> enum_value);

		bool click();

		const StringEnumPair str_pair_value;
		const std::weak_ptr<StringEnumSocketValue> enum_value;
	};

	class CurveEditModeArea : public Area {
	public:
		CurveEditModeArea(FloatPos begin_pos, FloatPos end_pos, EditCurveMode this_mode, EditCurveMode* mode_enum);

		bool click();

	private:
		EditCurveMode this_mode;
		EditCurveMode* mode_enum;
	};

	class CurveInterpModeArea : public Area {
	public:
		CurveInterpModeArea(FloatPos begin_pos, FloatPos end_pos, CurveInterpolation this_interp, CurveInterpolation* interp_enum);

		bool click();

	private:
		CurveInterpolation this_interp;
		CurveInterpolation* interp_enum;
	};

	class SocketArea : public Area {
	public:
		SocketArea(FloatPos begin_pos, FloatPos end_pos, std::weak_ptr<NodeSocket> socket);

		const std::weak_ptr<NodeSocket> socket;
	};

}
