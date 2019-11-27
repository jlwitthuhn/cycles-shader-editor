#pragma once

#include <memory>

#include "input_box.h"
#include "panel_edit.h"
#include "widget_multi_input.h"

namespace cse {
	class FloatSocketValue;

	class EditFloatPanel : public EditParamPanel {
	public:
		EditFloatPanel(float width);

		virtual bool is_active() const override;

		virtual float draw(NVGcontext* draw_context) override;

		virtual void set_mouse_local_position(FloatPos local_pos) override;

		virtual bool should_capture_input() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;
		virtual void deselect_input_box() override;

		virtual bool should_push_undo_state() override;

	private:
		std::weak_ptr<FloatSocketValue> attached_float;

		float input_widget_pos = 0.0f;
		MultiInputWidget input_widget;
	};
}