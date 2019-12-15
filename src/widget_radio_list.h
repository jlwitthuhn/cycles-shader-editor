#pragma once

#include <memory>

#include "float_pos.h"
#include "util_area.h"

struct NVGcontext;

namespace cse {
	class StringEnumSocketValue;

	// Widget that creates a radio button input panel for an enum
	class RadioListWidget {
	public:
		RadioListWidget(float width);

		void attach_enum(std::weak_ptr<StringEnumSocketValue> enum_socket_value);

		void pre_draw();
		float draw(NVGcontext* draw_context);

		void set_mouse_local_position(FloatPos local_pos);

		void handle_mouse_button(int button, int action, int mods);

		bool should_push_undo_state();

	private:
		const float width;

		std::weak_ptr<StringEnumSocketValue> enum_socket_value;
		std::vector<StringEnumArea> enum_targets;

		FloatPos mouse_local_pos;

		bool request_undo_push = false;
	};
}
