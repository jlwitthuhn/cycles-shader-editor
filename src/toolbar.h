#pragma once

#include <list>
#include <string>

#include "gui_sizes.h"
#include "ui_requests.h"
#include "util_vector.h"

struct NVGcontext;

namespace cse {

	enum class ToolbarButtonType {
		SAVE,
		UNDO,
		REDO,
		ZOOM_IN,
		ZOOM_OUT,
		SPACER,
	};

	class ToolbarButton {
	public:
		ToolbarButton(ToolbarButtonType type);

		std::string get_label() const;

		// Called from NodeEditorToolbar::draw to store the geometry of this button every draw
		void set_geometry(Float2 pos, float width, float height);
		bool contains_point(Float2 point) const;

		ToolbarButtonType type = ToolbarButtonType::SPACER;
		bool pressed = false;
		bool enabled = true;

	private:
		Float2 pos;
		float width;
		float height;
	};

	class NodeEditorToolbar {
	public:
		static float get_toolbar_height() { return UI_TOOLBAR_HEIGHT; }

		NodeEditorToolbar();

		void draw(NVGcontext* draw_context, float toolbar_width);

		void set_button_enabled(ToolbarButtonType type, bool enabled);

		bool is_mouse_over();
		void set_mouse_position(Float2 screen_position);
		void handle_mouse_button(int button, int action, int mods);

		UIRequests consume_ui_requests();

	private:
		void press_button_under_mouse();
		void release_button_under_mouse();
		void set_request(ToolbarButtonType button_type);

		Float2 mouse_screen_pos;

		std::list<ToolbarButton> buttons;

		UIRequests requests;
	};

}
