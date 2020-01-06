#pragma once

#include <memory>
#include <string>

#include "util_vector.h"

struct NVGcontext;

namespace cse {

	class EditableNode;

	class NodeCreationButton {
	public:
		virtual ~NodeCreationButton() {}

		virtual float draw(NVGcontext* draw_context, Float2 draw_origin, Float2 parent_local_mouse_pos, float parent_width);
		virtual bool is_mouse_over_button();

		virtual std::shared_ptr<EditableNode> create_node() = 0;

		bool pressed = false;

	protected:
		std::string label;

		Float2 mouse_parent_pos; // Mouse position, local within parent subwindow
		Float2 draw_pos;

		float button_width;
	};

	// Generic
	template<typename T>
	class GenericNodeButton : public NodeCreationButton {
	public:
		GenericNodeButton() {
			const Float2 irrelevant_position(0.0f, 0.0f);
			const T tmp_node(irrelevant_position);
			this->label = tmp_node.get_title();
		}

		virtual std::shared_ptr<EditableNode> create_node() override {
			const Float2 irrelevant_position(0.0f, 0.0f);
			return std::make_shared<T>(irrelevant_position);
		}
	};

}
