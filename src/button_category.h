#pragma once

#include <list>
#include <memory>
#include <string>

#include "buttons_nodes.h"
#include "gui_sizes.h"
#include "util_vector.h"

struct NVGcontext;

namespace cse {

	class NodeCategoryButtonPlacer {
	public:
		NodeCategoryButtonPlacer(Float2 draw_origin, float parent_width, float vertical_padding);
		Float2 next_button_position();
		float get_draw_height();

	private:
		Float2 draw_origin;
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

		void draw(Float2 draw_position, NVGcontext* draw_context);
		void update_mouse_position(Float2 local_position);

		bool is_mouse_over_button() const;

		bool selected = false;

		std::list<std::unique_ptr<NodeCreationButton>> node_buttons;

	private:
		std::string label;

		Float2 mouse_local_pos;

	};

}
