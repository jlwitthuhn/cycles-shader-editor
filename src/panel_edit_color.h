#pragma once

#include <memory>

#include "panel_edit.h"
#include "util_area.h"
#include "util_color.h"
#include "widget_multi_input.h"

struct NVGcontext;

namespace cse {

	class ColorSocketValue;
	class Float2;
	class SocketValue;

	class EditColorPanel : public EditParamPanel {
	public:
		EditColorPanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
		virtual float draw(NVGcontext* draw_context) override;

		virtual void set_mouse_local_position(Float2 local_pos) override;

		virtual bool should_capture_input() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;
		virtual void deselect_input_box() override;

		virtual bool should_push_undo_state() override;

	private:
		HueSatVal get_hsv();
		void set_hsv(HueSatVal hsv);

		void set_hue_from_mouse();
		void set_sat_val_from_mouse();

		bool mouse_sat_val_selection_active = false;
		bool mouse_hue_selection_active = false;

		Area color_rect_click_target;
		Area hue_bar_click_target;

		std::weak_ptr<ColorSocketValue> attached_color;

		float input_widget_pos = 0.0f;
		MultiInputWidget input_widget;

		float last_hue = 0.0f;
	};
}
