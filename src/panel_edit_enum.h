#pragma once

#include "panel_edit.h"
#include "widget_radio_list.h"

namespace cse {
	class StringEnumSocketValue;

	class EditEnumPanel : public EditParamPanel {
	public:
		EditEnumPanel(float width);

		virtual bool is_active() const override;

		virtual float draw(NVGcontext* draw_context) override;

		virtual void set_mouse_local_position(FloatPos local_pos) override;

		virtual void handle_mouse_button(int button, int action, int mods) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;

		virtual bool should_push_undo_state() override;

	private:
		std::weak_ptr<StringEnumSocketValue> attached_enum;

		float radio_widget_pos = 0.0f;
		RadioListWidget radio_widget;
	};
}
