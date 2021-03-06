#pragma once

#include <list>
#include <memory>

#include "button_category.h"
#include "buttons_nodes.h"
#include "subwindow.h"

struct NVGcontext;

namespace cse {

	class EditableNode;
	class Float2;

	class NodeCreationHelper {
	public:
		NodeCreationHelper();

		bool is_ready() const;

		void set_node(const std::shared_ptr<EditableNode>& new_node);
		void clear();
		std::shared_ptr<EditableNode> take();

	private:
		std::shared_ptr<EditableNode> current_node;
	};

	class NodeListSubwindow : public NodeEditorSubwindow {
	public:
		NodeListSubwindow(std::weak_ptr<NodeCreationHelper> node_creation_helper, Float2 screen_position);

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
