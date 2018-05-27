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

		void set_node(EditorNode* new_node);
		void clear();
		EditorNode* take();

	private:
		EditorNode* current_node = nullptr;
	};

	class NodeListSubwindow : public NodeEditorSubwindow {
	public:
		NodeListSubwindow(std::weak_ptr<NodeCreationHelper> node_creation_helper, FloatPos screen_position);
		virtual ~NodeListSubwindow();

		virtual void pre_draw() override;

		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void mouse_left_release() override;

		NodeCategoryButton* get_category_button_under_mouse();
		NodeCreationButton* get_button_under_mouse();

	protected:
		virtual void draw_content(NVGcontext* draw_context) override;

		void activate_creation_button(NodeCreationButton* node_button);
		void reset_creation_buttons();

		std::list<NodeCategoryButton*> category_buttons;

		NodeCategoryButton* active_category = nullptr;

		std::weak_ptr<NodeCreationHelper> node_creation_helper;
	};

}
