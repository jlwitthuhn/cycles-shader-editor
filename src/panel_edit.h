#pragma once

#include <memory>

#include "util_vector.h"

struct NVGcontext;

namespace cse {
	class SocketValue;

	class EditParamPanel {
	public:
		EditParamPanel(float width);
		virtual ~EditParamPanel() {}

		float get_width() const;
		float get_height() const;

		virtual bool is_active() const = 0;

		virtual void pre_draw();
		virtual float draw(NVGcontext* draw_context) = 0;

		virtual void set_mouse_local_position(Float2 local_pos);
		virtual bool is_mouse_over() const;

		virtual bool has_input_focus() const;
		virtual void handle_mouse_button(int button, int action, int mods);
		virtual void handle_key(int key, int scancode, int action, int mods);
		virtual void handle_character(unsigned int codepoint);

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) = 0;
		virtual void deselect_input_box();

		virtual bool should_push_undo_state();

		virtual void tab();

	protected:
		virtual void reset();

		const float panel_width;
		float panel_height = 1.0f;

		Float2 mouse_local_pos;

		bool request_undo_push = false;
	};
}
