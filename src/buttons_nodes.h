#pragma once

#include <memory>
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

		virtual std::unique_ptr<EditorNode> create_node() = 0;

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
			const T tmp_node(irrelevant_position);
			this->label = tmp_node.get_title();
		}

		virtual std::unique_ptr<EditorNode> create_node() override {
			const FloatPos irrelevant_position(0.0f, 0.0f);
			return std::make_unique<T>(irrelevant_position);
		}
	};

}