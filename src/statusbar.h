#pragma once

#include <string>

#include "gui_sizes.h"

struct NVGcontext;

namespace cse {

	class NodeEditorStatusBar {
	public:
		static float get_status_bar_height() { return UI_STATUSBAR_HEIGHT; }

		NodeEditorStatusBar();

		void draw(NVGcontext* draw_context, float width);

		void set_status_text(const std::string& text);
		void set_zoom_text(const std::string& text);

	private:
		std::string status_text;
		std::string zoom_text;
	};

}