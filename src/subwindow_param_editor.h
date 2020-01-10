#pragma once

#include <memory>
#include <vector>

#include "subwindow.h"

struct NVGcontext;

namespace cse {

	class NodeSocket;
	class EditParamPanel;
	class Float2;
	class Selection;

	class ParamEditorSubwindow : public NodeEditorSubwindow {
	public:
		ParamEditorSubwindow(Float2 screen_position);

		virtual void pre_draw() override;

		virtual void set_mouse_position(Float2 local_position, float max_pos_y) override;

		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

		virtual bool has_input_focus() const override;
		virtual void deselect_input_box() override;

		virtual bool is_active() const override;

		virtual bool needs_undo_push() override;
		virtual void update_selection(std::weak_ptr<const Selection> selection) override;

	protected:
		virtual void draw_content(NVGcontext* draw_context) override;

		std::weak_ptr<NodeSocket> selected_param;

		float panel_start_y = 0;

		// Panels
		std::vector<std::shared_ptr<EditParamPanel>> panels;

		// Undo state stuff
		bool request_undo_stack_push = false;
	};

}
