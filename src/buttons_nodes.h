#pragma once

#include <string>

#include "float_pos.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class EditorNode;

	class NodeCreationButton {
	public:
		virtual ~NodeCreationButton() {}

		virtual float draw(NVGcontext* draw_context, FloatPos draw_origin, FloatPos parent_local_mouse_pos, float parent_width);
		virtual bool is_mouse_over_button();

		// Caller is responsible for deleting the pointer after use
		virtual EditorNode* create_node(FloatPos world_position) = 0;

		bool pressed = false;

	protected:
		std::string label;

		FloatPos mouse_parent_pos; // Mouse position, local within parent subwindow
		FloatPos draw_pos;

		float button_width;
	};

	// Generic

	template<typename T>
	class GenericNodeButton : public NodeCreationButton {
	public:
		GenericNodeButton() {
			const FloatPos irrelevant_position(0.0f, 0.0f);
			T* const tmp_node = new T(irrelevant_position);
			this->label = tmp_node->get_title();
			delete tmp_node;
		}

		virtual EditorNode* create_node(const FloatPos world_position) override {
			return new T(world_position);
		}
	};

}