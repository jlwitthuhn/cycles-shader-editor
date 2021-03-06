#pragma once

#include <memory>
#include <vector>

#include "input_box.h"
#include "panel_edit.h"
#include "util_area.h"

struct NVGcontext;

namespace cse {

	class ColorSocketValue;
	class ColorRampSocketValue;
	class EditColorPanel;
	class Float2;
	class FloatSocketValue;
	class SocketValue;

	struct ColorRampPoint;

	class EditColorRampPanel : public EditParamPanel {
	public:
		EditColorRampPanel(float width);
		virtual ~EditColorRampPanel() override;

		virtual bool is_active() const override;

		virtual void pre_draw() override;
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
		void update_preview();
		bool something_is_selected() const;

		// Everything needed to support one row in the UI
		class ColorRampRow {
		public:
			ColorRampRow(cse::ColorRampPoint point);

			bool has_input_focus() const;
			void handle_character(unsigned int codepoint);

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

		static constexpr int PREVIEW_WIDTH_PX = 256;
		static constexpr int PREVIEW_CHANNELS = 4;
		unsigned char preview_tex_color[PREVIEW_WIDTH_PX * PREVIEW_CHANNELS];
		unsigned char preview_tex_alpha[PREVIEW_WIDTH_PX * PREVIEW_CHANNELS];

		NVGcontext* nvg_context = nullptr;
		int nvg_handle_preview_color = -1;
		int nvg_handle_preview_alpha = -1;
	};
}
