#pragma once

#include <list>
#include <string>

#include "float_pos.h"
#include "gui_sizes.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class NodeCreationButton;

	class NodeCategoryButtonPlacer {
	public:
		NodeCategoryButtonPlacer(FloatPos draw_origin, float parent_width, float vertical_padding);
		FloatPos next_button_position();
		float get_draw_height();

	private:
		FloatPos draw_origin;
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

		NodeCategoryButton(const std::string& label);
		~NodeCategoryButton();

		void draw(FloatPos draw_position, NVGcontext* draw_context);
		void update_mouse_position(FloatPos local_position);

		bool is_mouse_over_button();

		bool pressed = false;

		std::list<NodeCreationButton*> node_buttons;

	private:
		std::string label;

		FloatPos mouse_local_pos;

	};

}
