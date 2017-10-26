#pragma once

#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class NodeEditorSubwindow {
	public:
		NodeEditorSubwindow(Point2 screen_position);
		virtual ~NodeEditorSubwindow() {}

		virtual Point2 get_screen_pos() const;

		// Draws the window at (0, 0). Can freely modify nanovg state without nvgSave/nvgRestore.
		virtual void draw(NVGcontext* draw_context) = 0;
		virtual bool is_mouse_over();
		virtual void set_mouse_position(Point2 screen_position);

		virtual void handle_mouse_button(int button, int action, int mods);
		virtual void handle_key(int key, int scancode, int action, int mods);
		virtual void handle_character(unsigned int codepoint);

		virtual void move_window_begin();
		virtual void move_window_end();

		virtual bool is_subwindow_active() { return true; }

	protected:
		Point2 subwindow_screen_pos;

		Point2 mouse_local_pos;
		Point2 mouse_local_begin_move_pos;

		float subwindow_width = 10.0f;
		float subwindow_height = 10.0f;

		bool subwindow_moving = false;
	};

}
