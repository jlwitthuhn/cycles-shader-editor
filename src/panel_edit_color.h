#pragma once

#include <memory>

#include "input_box.h"
#include "panel_edit.h"
#include "util_area.h"
#include "util_color.h"

struct NVGcontext;

namespace cse {
	class ColorSocketValue;
	class SocketValue;

	class EditColorPanel : public ParamEditorPanel {
	public:
		EditColorPanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
		virtual float draw(NVGcontext* draw_context) override;

		virtual bool should_capture_input() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;
		virtual void deselect_input_box() override;

	protected:
		virtual void reset() override;

	private:
		HueSatVal get_hsv();
		void set_hsv(HueSatVal hsv);

		void set_hue_from_mouse();
		void set_sat_val_from_mouse();

		void select_input(FloatInputBox* input);

		std::weak_ptr<ColorSocketValue> attached_color;
		FloatInputBox* selected_input = nullptr;

		float last_hue = 0.0f;

		FloatInputBox color_r_input_box;
		FloatInputBox color_g_input_box;
		FloatInputBox color_b_input_box;

		bool mouse_sat_val_selection_active = false;
		bool mouse_hue_selection_active = false;

		Area color_rect_click_target;
		Area hue_bar_click_target;
	};
}
