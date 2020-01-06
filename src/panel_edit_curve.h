#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "common_enums.h"
#include "panel_edit.h"
#include "util_area.h"
#include "util_vector.h"

struct NVGcontext;

namespace cse {
	class CurveSocketValue;
	class SocketValue;

	class EditCurvePanel : public EditParamPanel {
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
		void move_selected_point(Float2 new_pos);

		std::weak_ptr<CurveSocketValue> attached_curve;
		EditCurveMode edit_mode = EditCurveMode::MOVE;

		Area target_view;
		std::vector<HolderArea<EditCurveMode>> edit_mode_click_areas;
		std::vector<HolderArea<CurveInterpolation>> interp_click_areas;

		std::size_t selected_point_index = 0;
		bool selected_point_valid = false;

		bool moving_selected_point = false;
		bool mouse_has_moved = false;
		Float2 move_selected_point_begin_mouse_pos;
	};
}
