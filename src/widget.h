#pragma once

#include "util_vector.h"

struct NVGcontext;

namespace cse {

	class Widget {
	public:
		Widget(float width);
		virtual ~Widget();

		virtual float draw(NVGcontext* draw_context);

		void set_mouse_local_pos(Float2 local_pos);

		virtual bool has_input_focus() const;
		virtual void handle_mouse_button(int button, int action, int mods);
		virtual void handle_key(int key, int scancode, int action, int mods);
		virtual void handle_character(unsigned int codepoint);

		virtual bool should_push_undo_state();

	protected:
		const float width;

		Float2 mouse_pos;

		bool request_undo_push = false;
	};
}
