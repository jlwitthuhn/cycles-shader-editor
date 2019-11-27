#pragma once

#include <memory>
#include <vector>

#include "input_box.h"
#include "panel_edit.h"
#include "util_area.h"
#include "util_color_ramp.h"

namespace cse {
	class ColorRampSocketValue;
	class EditColorPanel;

	class EditColorRampPanel : public EditParamPanel {
	public:
		EditColorRampPanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
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
		// Everything needed to support one row in the UI
		class ColorRampRow {
		public:
			ColorRampRow(cse::ColorRampPoint point);

			FloatInputBox box_pos;
			FloatInputBox box_alpha;
			Area color_target;
			bool color_selected = false;

			Area delete_target;

			std::shared_ptr<FloatSocketValue> value_pos;
			std::shared_ptr<FloatSocketValue> value_alpha;
			std::shared_ptr<ColorSocketValue> value_color;

			bool flagged_for_deletion = false;
		};

		std::weak_ptr<ColorRampSocketValue> attached_ramp;

		std::vector<ColorRampRow> ramp_rows;
		Area new_point_button;
		bool new_point_button_pressed = false;

		float edit_color_panel_pos = 0.0f;
		std::shared_ptr<EditColorPanel> edit_color_panel;
	};
}
