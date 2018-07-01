#pragma once

#include <list>
#include <memory>
#include <vector>

#include "input_box.h"
#include "subwindow.h"
#include "util_area.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class FloatPos;
	class NodeSocket;
	class ParamEditorPanel;
	class Selection;

	class ParamEditorSubwindow : public NodeEditorSubwindow {
	public:
		ParamEditorSubwindow(FloatPos screen_position);

		virtual void pre_draw() override;

		virtual void set_mouse_position(FloatPos local_position, float max_pos_y) override;

		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual bool should_capture_input() const override;
		virtual void deselect_input_box() override;

		virtual bool is_active() const override;

		virtual bool needs_undo_push() override;
		virtual void update_selection(std::weak_ptr<const Selection> selection) override;

	protected:
		virtual void draw_content(NVGcontext* draw_context) override;

		void select_input_box(BaseInputBox* input);

		bool is_bool_target_under_mouse();
		bool is_enum_target_under_mouse();
		void click_bool_target_under_mouse();
		void click_enum_target_under_mouse();

		std::weak_ptr<NodeSocket> selected_param;
		BaseInputBox* selected_input = nullptr;

		float panel_start_y = 0;

		// Panels
		std::list<std::shared_ptr<ParamEditorPanel>> panels;

		// Int stuff
		IntInputBox int_input_box;

		// Float stuff
		FloatInputBox float_input_box;

		// Vector stuff
		FloatInputBox vector_x_input_box;
		FloatInputBox vector_y_input_box;
		FloatInputBox vector_z_input_box;

		// Enum stuff
		std::vector<StringEnumArea> enum_targets;

		// Bool stuff
		std::vector<BoolValueArea> bool_targets;

		// Undo state stuff
		bool request_undo_stack_push = false;
	};

}
