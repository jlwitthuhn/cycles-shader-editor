#pragma once

#include "click_target.h"
#include "common_enums.h"

struct NVGcontext;

namespace CyclesShaderEditor {
	class CurveSocketValue;

	class EditCurvePanel {
	public:
		EditCurvePanel(float width);

		bool is_active();
		void set_attached_curve_value(CurveSocketValue* curve_value);

		void reset_panel_state();

		void pre_draw();
		float draw(NVGcontext* draw_context);
		void set_mouse_local_position(Point2 local_pos);
		bool is_mouse_over();

		void handle_mouse_button(int button, int action, int mods);
		void mouse_button_release();

		bool should_push_undo_state();

	private:
		CurveSocketValue* attached_curve = nullptr;
		EditCurveMode edit_mode = EditCurveMode::MOVE;

		Point2 mouse_local_pos;

		float panel_width = 1.0f;
		float panel_height = 1.0f;

		GenericClickTarget target_view;
		CurveEditModeClickTarget target_edit_mode_move;
		CurveEditModeClickTarget target_edit_mode_create;
		CurveEditModeClickTarget target_edit_mode_delete;
		CurveInterpClickTarget target_interp_linear;
		CurveInterpClickTarget target_interp_hermite;

		size_t selected_point_index = 0;
		bool selected_point_valid = false;

		bool move_selected_point = false;
		bool mouse_has_moved = false;
		Point2 move_selected_point_begin_mouse_pos;

		bool request_undo_push = false;
	};
}
