// Common drawing functions go here
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "util_area.h"

struct NVGcontext;

namespace cse {

	class Float2;
	class Float3;
	class NodeSocket;

	namespace Drawing {

		// UI Elements
		void draw_button(NVGcontext* draw_context, Float2 pos, float width, float height, const std::string& label, bool enabled, bool pressed);
		void draw_color_swatch(NVGcontext* draw_context, Float2 pos, float width, float height, Float3 color, bool selected);
		void draw_color_pick_cursor(NVGcontext* draw_context, Float2 pos);

		// Node Graph View
		float draw_node(
			NVGcontext* draw_context,
			const char* title,
			Float3 header_color,
			float node_width,
			Float2 world_pos,
			bool selected,
			const std::vector<std::shared_ptr<NodeSocket>>& socket_vec,
			const std::shared_ptr<cse::NodeSocket>& selected_socket,
			std::vector<HolderArea<std::weak_ptr<NodeSocket>>>& socket_targets,
			std::vector<HolderArea<std::weak_ptr<NodeSocket>>>& label_targets
		);
		void draw_node_connection_curve(NVGcontext* draw_context, Float2 begin_pos, Float2 end_pos, float width);

	}
}
