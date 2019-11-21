#pragma once

#include <cstddef>
#include <memory>

#include "common_enums.h"
#include "float_pos.h"
#include "panel_edit.h"
#include "util_area.h"

struct NVGcontext;

namespace cse {
	class CurveSocketValue;
	class SocketValue;

	class EditCurvePanel : public ParamEditorPanel {
	public:
		EditCurvePanel(float width);

		virtual bool is_active() const override;

		virtual void pre_draw() override;
		virtual float draw(NVGcontext* draw_context) override;
		virtual void handle_mouse_button(int button, int action, int mods) override;

		virtual void set_attached_value(std::weak_ptr<SocketValue> socket_value) override;

	protected:
		virtual void reset() override;

	private:
		void move_selected_point(FloatPos new_pos);

		std::weak_ptr<CurveSocketValue> attached_curve;
		EditCurveMode edit_mode = EditCurveMode::MOVE;

		Area target_view;
		CurveEditModeArea target_edit_mode_move;
		CurveEditModeArea target_edit_mode_create;
		CurveEditModeArea target_edit_mode_delete;
		CurveInterpModeArea target_interp_linear;
		CurveInterpModeArea target_interp_hermite;

		std::size_t selected_point_index = 0;
		bool selected_point_valid = false;

		bool moving_selected_point = false;
		bool mouse_has_moved = false;
		FloatPos move_selected_point_begin_mouse_pos;
	};
}
