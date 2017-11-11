// Common drawing functions go here
#pragma once

#include <string>

struct NVGcontext;

namespace CyclesShaderEditor {

	namespace Drawing {

		void draw_button(NVGcontext* draw_context, float x_pos, float y_pos, float width, float height, std::string label, bool enabled, bool pressed);

	}
}
