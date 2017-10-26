#pragma once

#include "gui_sizes.h"
#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class NodeEditorStatusBar {
	public:
		static float get_status_bar_height() { return UI_STATUSBAR_HEIGHT; }

		NodeEditorStatusBar();

		void draw(NVGcontext* draw_context, float width);

		void set_status_text(std::string text);
		void set_zoom_text(std::string text);

	private:
		std::string status_text;
		std::string zoom_text;
	};

}