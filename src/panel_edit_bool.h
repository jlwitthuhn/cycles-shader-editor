#pragma once

#include <memory>

#include "panel_edit.h"
#include "widget_radio_list.h"

struct NVGcontext;

namespace cse {

	class BoolSocketValue;
	class Float2;
	class SocketValue;

	class EditBoolPanel : public EditParamPanel {
	public:
		EditBoolPanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
		virtual float draw(NVGcontext* draw_context) override;

		virtual void set_mouse_local_position(Float2 local_pos) override;

		virtual void handle_mouse_button(int button, int action, int mods) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;

		virtual bool should_push_undo_state() override;

	private:
		std::weak_ptr<BoolSocketValue> attached_bool;

		float radio_widget_pos = 0.0f;
		RadioListWidget radio_widget;
	};
}
