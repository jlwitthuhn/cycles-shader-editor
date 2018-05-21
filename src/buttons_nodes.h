#pragma once

#include <string>

#include "point2.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class EditorNode;

	class NodeCreationButton {
	public:
		virtual ~NodeCreationButton() {}

		virtual float draw(NVGcontext* draw_context, Point2 draw_origin, Point2 parent_local_mouse_pos, float parent_width);
		virtual bool is_mouse_over_button();

		// Caller is responsible for deleting the pointer after use
		virtual EditorNode* create_node(Point2 world_position) = 0;

		bool pressed = false;

	protected:
		std::string label;

		Point2 mouse_parent_pos; // Mouse position, local within parent subwindow
		Point2 draw_pos;

		float button_width;
	};

	// Generic

	template<typename T>
	class GenericNodeButton : public NodeCreationButton {
	public:
		GenericNodeButton() {
			const Point2 irrelevant_position(0.0f, 0.0f);
			T* const tmp_node = new T(irrelevant_position);
			this->label = tmp_node->get_title();
			delete tmp_node;
		}

		virtual EditorNode* create_node(const Point2 world_position) override {
			return new T(world_position);
		}
	};

}