// Common drawing functions go here
#pragma once

#include <string>

struct NVGcontext;

namespace cse {

	class FloatPos;

	namespace Drawing {

		void draw_button(NVGcontext* draw_context, FloatPos pos, float width, float height, const std::string& label, bool enabled, bool pressed);
		void draw_color_pick_cursor(NVGcontext* draw_context, FloatPos pos);
		void draw_node_connection_curve(NVGcontext* draw_context, FloatPos begin_pos, FloatPos end_pos, float width);

	}
}
