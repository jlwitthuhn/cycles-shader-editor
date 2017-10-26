#pragma once

#include <list>
#include <string>

#include "gui_sizes.h"
#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class NodeCreationButton;

	class NodeCategoryButtonPlacer {
	public:
		NodeCategoryButtonPlacer(Point2 draw_origin, float parent_width, float vertical_padding);
		Point2 next_button_position();
		float get_draw_height();

	private:
		Point2 draw_origin;
		float button_width;
		float button_height;
		float parent_width;
		float vertical_padding;

		int positions_made;
	};

	class NodeCategoryButton {
	public:
		static float get_button_width() { return UI_SUBWIN_NODE_LIST_CAT_BUTTON_WIDTH; }
		static float get_button_height() { return UI_SUBWIN_NODE_LIST_CAT_BUTTON_HEIGHT; }

		NodeCategoryButton(std::string label);
		~NodeCategoryButton();

		void draw(Point2 draw_position, NVGcontext* draw_context);
		void update_mouse_position(Point2 local_position);

		bool is_mouse_over_button();

		bool pressed = false;

		std::list<NodeCreationButton*> node_buttons;

	private:
		std::string label;

		Point2 mouse_local_pos;

	};

}
