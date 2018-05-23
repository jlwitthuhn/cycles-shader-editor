// Common drawing functions go here
#pragma once

#include <string>

#include "float_pos.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	namespace Drawing {

		void draw_button(NVGcontext* draw_context, FloatPos pos, float width, float height, const std::string& label, bool enabled, bool pressed);
		void draw_color_pick_cursor(NVGcontext* draw_context, FloatPos pos);

	}
}
