#pragma once

#include "click_target.h"
#include "float_pos.h"
#include "input_box.h"
#include "util_color.h"

struct NVGcontext;

namespace CyclesShaderEditor {
	class ColorSocketValue;

	class EditColorPanel {
	public:
		EditColorPanel(float width);

		bool is_active();
		void set_active(bool active);

		float draw(NVGcontext* draw_context, ColorSocketValue* socket_value);
		void set_mouse_local_position(FloatPos local_pos);
		bool is_mouse_over();

		void handle_mouse_button(int button, int action, int mods);
		void mouse_button_release();

		BaseInputBox* get_input_bux_under_mouse();

		bool should_push_undo_state();

	private:
		HueSatVal get_hsv();
		void set_hsv(HueSatVal hsv);

		void set_hue_from_mouse();
		void set_sat_val_from_mouse();

		float panel_width = 1.0f;
		float panel_height = 1.0f;
		bool active = false;

		float last_hue = 0.0f;

		FloatInputBox color_r_input_box;
		FloatInputBox color_g_input_box;
		FloatInputBox color_b_input_box;

		bool value_has_changed = false;

		bool mouse_sat_val_selection_active = false;
		bool mouse_hue_selection_active = false;

		FloatPos mouse_local_pos;

		GenericClickTarget color_rect_click_target;
		GenericClickTarget hue_bar_click_target;
	};
}
