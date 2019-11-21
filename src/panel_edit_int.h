#pragma once

#include <memory>

#include "input_box.h"
#include "panel_edit.h"

namespace cse {
	class IntSocketValue;

	class EditIntPanel : public ParamEditorPanel {
	public:
		EditIntPanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
		virtual float draw(NVGcontext* draw_context) override;

		virtual bool should_capture_input() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;
		virtual void deselect_input_box() override;

	private:
		std::weak_ptr<IntSocketValue> attached_int;
		IntInputBox int_input_box;
	};
}