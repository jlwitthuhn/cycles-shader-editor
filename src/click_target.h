#pragma once

#include "point2.h"
#include "sockets.h"

namespace CyclesShaderEditor {

	class GenericClickTarget {
	public:
		GenericClickTarget(Point2 begin_pos, Point2 end_pos);

		bool is_mouse_over_target(Point2 mouse_pos);

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
		StringEnumClickTarget(Point2 begin_pos, Point2 end_pos, StringEnumPair str_pair, StringEnumSocketValue* enum_value);

		void click();

		StringEnumPair str_pair_value;
		StringEnumSocketValue* enum_value;
	};

	class SocketClickTarget : public GenericClickTarget {
	public:
		SocketClickTarget(Point2 begin_pos, Point2 end_pos, NodeSocket* socket);

		NodeSocket* socket;
	};

}
