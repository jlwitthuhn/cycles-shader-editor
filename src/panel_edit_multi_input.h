#pragma once

#include <memory>

#include "panel_edit.h"
#include "widget_multi_input.h"

struct NVGcontext;

namespace cse {

	class Float2;
	class FloatSocketValue;
	class Float3SocketValue;
	class IntSocketValue;
	class SocketValue;

	class EditMultiInputPanel : public EditParamPanel {
	public:
		EditMultiInputPanel(float width);

		virtual bool is_active() const override;

		virtual float draw(NVGcontext* draw_context) override;

		virtual void set_mouse_local_position(Float2 local_pos) override;

		virtual bool has_input_focus() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;
		virtual void deselect_input_box() override;

		virtual bool should_push_undo_state() override;

		virtual void tab() override;

	private:
		std::weak_ptr<IntSocketValue> attached_int;
		std::weak_ptr<FloatSocketValue> attached_float;
		std::weak_ptr<Float3SocketValue> attached_vec;

		float input_widget_pos = 0.0f;
		MultiInputWidget input_widget;
	};
}
