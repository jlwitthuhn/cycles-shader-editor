#pragma once

#include "common_enums.h"
#include "point2.h"
#include "sockets.h"

namespace CyclesShaderEditor {

	class GenericClickTarget {
	public:
		GenericClickTarget(Point2 begin_pos, Point2 end_pos);

		bool is_mouse_over_target(Point2 mouse_pos);
		Point2 get_normalized_mouse_pos(Point2 mouse_pos);

	protected:
		Point2 begin;
		Point2 end;
	};

	class BoolValueClickTarget : public GenericClickTarget {
	public:
		BoolValueClickTarget(Point2 begin_pos, Point2 end_pos, bool bool_value, BoolSocketValue* socket_value);

		void click();

		bool bool_value;
		BoolSocketValue* socket_value;
	};

	class StringEnumClickTarget : public GenericClickTarget {
	public:
		StringEnumClickTarget(Point2 begin_pos, Point2 end_pos, const StringEnumPair& str_pair, StringEnumSocketValue* enum_value);

		void click();

		StringEnumPair str_pair_value;
		StringEnumSocketValue* enum_value;
	};

	class CurveEditModeClickTarget : public GenericClickTarget {
	public:
		CurveEditModeClickTarget(Point2 begin_pos, Point2 end_pos, EditCurveMode this_mode, EditCurveMode* mode_enum);

		void click();

	private:
		EditCurveMode this_mode;
		EditCurveMode* mode_enum;
	};

	class CurveInterpClickTarget : public GenericClickTarget {
	public:
		CurveInterpClickTarget(Point2 begin_pos, Point2 end_pos, CurveInterpolation this_interp, CurveInterpolation* interp_enum);

		void click();

	private:
		CurveInterpolation this_interp;
		CurveInterpolation* interp_enum;
	};

	class SocketClickTarget : public GenericClickTarget {
	public:
		SocketClickTarget(Point2 begin_pos, Point2 end_pos, NodeSocket* socket);

		NodeSocket* socket;
	};

}
