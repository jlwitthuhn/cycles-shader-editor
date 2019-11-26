#pragma once

#include <memory>

#include "common_enums.h"
#include "float_pos.h"
#include "sockets.h"

namespace cse {

	class Area {
	public:
		Area(FloatPos begin_pos, FloatPos end_pos);

		bool is_under_point(FloatPos pos) const;
		FloatPos get_normalized_mouse_pos(FloatPos mouse_pos);

	protected:
		FloatPos begin;
		FloatPos end;
	};

	class BoolValueArea : public Area {
	public:
		BoolValueArea(FloatPos begin_pos, FloatPos end_pos, bool bool_value, std::weak_ptr<BoolSocketValue> socket_value);

		bool click();

		const bool bool_value;
		const std::weak_ptr<BoolSocketValue> socket_value;
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
