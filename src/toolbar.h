#pragma once

#include <string>
#include <vector>

#include "gui_sizes.h"
#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class UIRequests;

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

		std::string get_label();

		ToolbarButtonType type = ToolbarButtonType::SPACER;
		bool pressed = false;
		bool enabled = true;
	};

	class NodeEditorToolbar {
	public:
		static float get_toolbar_height() { return UI_TOOLBAR_HEIGHT; }

		NodeEditorToolbar(UIRequests* requests);

		void draw(NVGcontext* draw_context, float toolbar_width);
		bool is_mouse_over();
		void release_buttons();

		void disable_button(ToolbarButtonType type);
		void enable_button(ToolbarButtonType type);

		void set_mouse_position(Point2 screen_position);
		void handle_mouse_button(int button, int action, int mods);

	private:
		ToolbarButton* get_button_under_mouse();
		void set_request(ToolbarButtonType button_type);

		Point2 mouse_screen_pos;

		std::vector<ToolbarButton> buttons;

		UIRequests* requests = nullptr;
	};

}