#pragma once

#include "common_enums.h"
#include "float_pos.h"
#include "sockets.h"

namespace CyclesShaderEditor {

	class GenericClickTarget {
	public:
		GenericClickTarget(FloatPos begin_pos, FloatPos end_pos);

		bool is_mouse_over_target(FloatPos mouse_pos) const;
		FloatPos get_normalized_mouse_pos(FloatPos mouse_pos);

	protected:
		FloatPos begin;
		FloatPos end;
	};

	class BoolValueClickTarget : public GenericClickTarget {
	public:
		BoolValueClickTarget(FloatPos begin_pos, FloatPos end_pos, bool bool_value, BoolSocketValue* socket_value);

		void click();

		bool bool_value;
		BoolSocketValue* socket_value;
	};

	class StringEnumClickTarget : public GenericClickTarget {
	public:
		StringEnumClickTarget(FloatPos begin_pos, FloatPos end_pos, const StringEnumPair& str_pair, StringEnumSocketValue* enum_value);

		void click();

		StringEnumPair str_pair_value;
		StringEnumSocketValue* enum_value;
	};

	class CurveEditModeClickTarget : public GenericClickTarget {
	public:
		CurveEditModeClickTarget(FloatPos begin_pos, FloatPos end_pos, EditCurveMode this_mode, EditCurveMode* mode_enum);

		void click();

	private:
		EditCurveMode this_mode;
		EditCurveMode* mode_enum;
	};

	class CurveInterpClickTarget : public GenericClickTarget {
	public:
		CurveInterpClickTarget(FloatPos begin_pos, FloatPos end_pos, CurveInterpolation this_interp, CurveInterpolation* interp_enum);

		void click();

	private:
		CurveInterpolation this_interp;
		CurveInterpolation* interp_enum;
	};

	class SocketClickTarget : public GenericClickTarget {
	public:
		SocketClickTarget(FloatPos begin_pos, FloatPos end_pos, NodeSocket* socket);

		NodeSocket* socket;
	};

}
