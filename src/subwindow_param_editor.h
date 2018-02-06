#pragma once

#include <vector>

#include "input_box.h"
#include "panel_edit_color.h"
#include "panel_edit_curve.h"
#include "subwindow.h"

namespace CyclesShaderEditor {

	class BoolValueClickTarget;
	class NodeSocket;
	class StringEnumClickTarget;

	class ParamEditorSubwindow : public NodeEditorSubwindow {
	public:
		ParamEditorSubwindow(Point2 screen_position);

		virtual void draw(NVGcontext* draw_context) override;
		virtual void set_mouse_position(Point2 screen_position, float max_pos_y) override;

		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual bool is_subwindow_active() override;

		void set_selected_param(NodeSocket* selected_param);

		bool should_capture_keys();
		void complete_input();
		void mouse_button_release();

		bool should_push_undo_state();

	private:
		bool is_mouse_over_header();

		void select_input(BaseInputBox* input);

		bool is_bool_target_under_mouse();
		bool is_enum_target_under_mouse();
		void click_bool_target_under_mouse();
		void click_enum_target_under_mouse();

		NodeSocket* selected_param = nullptr;

		BaseInputBox* selected_input = nullptr;

		float panel_start_y = 0;

		// Color panel
		EditColorPanel panel_color;

		// Curve panel
		EditCurvePanel panel_curve;

		// Int stuff
		IntInputBox int_input_box;

		// Float stuff
		FloatInputBox float_input_box;

		// Vector stuff
		FloatInputBox vector_x_input_box;
		FloatInputBox vector_y_input_box;
		FloatInputBox vector_z_input_box;

		// Enum stuff
		std::vector<StringEnumClickTarget> enum_targets;

		// Bool stuff
		std::vector<BoolValueClickTarget> bool_targets;

		// Undo state stuff
		bool request_undo_stack_push = false;
	};

}
