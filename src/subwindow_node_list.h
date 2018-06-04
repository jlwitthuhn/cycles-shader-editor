#pragma once

#include <list>
#include <memory>

#include "subwindow.h"

namespace CyclesShaderEditor {

	class EditorNode;
	class NodeCategoryButton;
	class NodeCreationButton;

	class NodeCreationHelper {
	public:
		NodeCreationHelper();

		bool is_ready() const;

		void set_node(std::unique_ptr<EditorNode>& new_node);
		void clear();
		std::unique_ptr<EditorNode> take();

	private:
		std::unique_ptr<EditorNode> current_node;
	};

	class NodeListSubwindow : public NodeEditorSubwindow {
	public:
		NodeListSubwindow(std::weak_ptr<NodeCreationHelper> node_creation_helper, FloatPos screen_position);

		virtual void handle_mouse_button(int button, int action, int mods) override;

	protected:
		virtual void draw_content(NVGcontext* draw_context) override;

		const std::list<std::unique_ptr<NodeCreationButton>>& get_active_creation_buttons() const;
		bool is_category_button_under_mouse() const;
		bool is_creation_button_under_mouse() const;
		void select_category_button_under_mouse();
		void select_creation_button_under_mouse();

		void reset_creation_buttons();

		std::list<std::unique_ptr<NodeCategoryButton>> category_buttons;

		std::weak_ptr<NodeCreationHelper> node_creation_helper;

		// this is here so get_active_creation_buttons() can return an empty list in the case where no button is selected
		const std::list<std::unique_ptr<NodeCreationButton>> empty_creation_button_list;
	};

}
