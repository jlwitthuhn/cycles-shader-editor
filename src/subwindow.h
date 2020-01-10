#pragma once

#include <memory>
#include <string>

#include "util_vector.h"

struct NVGcontext;

namespace cse {

	class Selection;

	class NodeEditorSubwindow {
	public:
		NodeEditorSubwindow(Float2 screen_position, std::string title);
		virtual ~NodeEditorSubwindow() {}

		virtual Float2 get_screen_pos() const;
		virtual float get_width() const;
		virtual float get_height() const;

		virtual void pre_draw();
		// Draws the window at (0, 0). Can freely modify nanovg state without nvgSave/nvgRestore.
		virtual void draw(NVGcontext* draw_context);

		virtual bool is_mouse_over() const;
		virtual bool is_mouse_over_header() const;
		virtual void set_mouse_position(Float2 local_position, float max_pos_y);

		// Returns true if this subwindow wants to capture keyboard input
		virtual bool has_input_focus() const;
		virtual void deselect_input_box();

		virtual void handle_mouse_button(int button, int action, int mods);
		virtual void handle_key(int key, int scancode, int action, int mods);
		virtual void handle_character(unsigned int codepoint);

		virtual void move_window_begin();
		virtual void move_window_end();

		// Returns true if changes made by this window require an undo stack push
		// Currently only used by the param editor subwindow
		virtual bool needs_undo_push();
		virtual void update_selection(std::weak_ptr<const Selection> selection);

		virtual bool is_active() const { return true; }

	protected:
		virtual void draw_content(NVGcontext* draw_context) = 0;

		std::string title = "Subwindow";
		Float2 subwindow_screen_pos;

		Float2 mouse_local_pos; // Mouse position relative to the top-left corner of the window, prefer using mouse_content_pos over this
		Float2 mouse_local_begin_move_pos;
		Float2 mouse_content_pos; // Local position not including window header

		float subwindow_width = 100.0f;
		float content_height = 1.0f;

		bool subwindow_moving = false;
	};

}
