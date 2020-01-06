// Common drawing functions go here
#pragma once

#include <string>

#include <nanovg.h>

#include "util_vector.h"

namespace cse {

	namespace Drawing {

		void draw_button(NVGcontext* draw_context, Float2 pos, float width, float height, const std::string& label, bool enabled, bool pressed);
		void draw_color_swatch(NVGcontext* draw_context, Float2 pos, float width, float height, NVGcolor color, bool selected);
		void draw_color_pick_cursor(NVGcontext* draw_context, Float2 pos);
		void draw_node_connection_curve(NVGcontext* draw_context, Float2 begin_pos, Float2 end_pos, float width);

	}
}
