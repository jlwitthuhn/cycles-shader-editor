#pragma once

#include <memory>

#include "float_pos.h"

struct NVGcontext;

namespace CyclesShaderEditor {
	class SocketValue;

	class ParamEditorPanel {
	public:
		ParamEditorPanel(float width);
		virtual ~ParamEditorPanel() {}

		virtual bool is_active() const = 0;

		virtual void pre_draw();
		virtual float draw(NVGcontext* draw_context) = 0;

		virtual void set_mouse_local_position(FloatPos local_pos);
		virtual bool is_mouse_over();

		virtual bool should_capture_input() const;
		virtual void handle_mouse_button(int button, int action, int mods);
		virtual void handle_key(int key, int scancode, int action, int mods);
		virtual void handle_character(unsigned int codepoint);

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) = 0;
		virtual void deselect_input_box();

		virtual bool should_push_undo_state();

	protected:
		virtual void reset();

		float panel_width;
		float panel_height = 1.0f;

		FloatPos mouse_local_pos;
	};
}
