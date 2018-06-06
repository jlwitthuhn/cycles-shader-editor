#pragma once

#include <memory>
#include <set>
#include <string>

#include "float_pos.h"
#include "zoom.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class EditableGraph;
	class EditorNode;
	class NodeCreationHelper;
	class NodeSocket;
	class Selection;

	enum class SelectMode {
		EXCLUSIVE,
		ADD,
		TOGGLE,
		NONE,
	};

	class EditGraphView {
	private:
		class ViewBorders {
		public:
			ViewBorders(FloatPos view_center, int viewport_width, int viewport_height, float zoom_scale);

			const float left;
			const float right;
			const float top;
			const float bottom;
		};

	public:
		EditGraphView(
			EditableGraph* graph,
			std::weak_ptr<NodeCreationHelper> node_creation_helper
		);

		// Below functions query the state of the view
		EditorNode* get_node_under_mouse();
		NodeSocket* get_socket_label_under_mouse();
		NodeSocket* get_selected_socket_label();
		NodeSocket* get_socket_connector_under_mouse();
		FloatPos get_mouse_world_position();
		bool is_node_under_mouse_selected();
		std::string get_zoom_string();

		// Main 3 functions called from the main loop
		void set_mouse_position(FloatPos view_local_mouse_pos, int viewport_width, int viewport_height);
		void update();
		void draw(NVGcontext* draw_context);

		// Input
		void handle_mouse_button(int button, int action, int mods);
		void handle_key(int key, int scancode, int action, int mods);

		// Undo
		bool needs_undo_push();

		// Misc
		std::weak_ptr<const Selection> get_const_selection() const;

		// Functions to manipulate view state
		// TODO: remove or make private all functions below here
		void add_node_at_mouse(std::unique_ptr<EditorNode>& node);
		void begin_connection_under_mouse();
		void cancel_connection();
		void complete_connection_at_mouse();
		void clear_input_socket_under_mouse();
		void clear_node_selection();

		void delete_selected_nodes();
		void delete_node(EditorNode* node);

		void node_move_begin();
		void node_move_end();

		void move_left();
		void move_right();
		void move_up();
		void move_down();

		void zoom_in();
		void zoom_out();

		void box_select_begin();
		void box_select_end(SelectMode mode);
		
		void select_label_under_mouse();
		void deselect_label();

		void raise_node_under_mouse(SelectMode mode);

	private:
		void select_label(NodeSocket* label);

		void raise_node(EditorNode* node, SelectMode mode);

		std::set<EditorNode*> get_boxed_nodes();

		// This points to an object owned by MainWindow
		// It will be valid for the full life of the EditGraphView
		EditableGraph* const graph;

		std::weak_ptr<NodeCreationHelper> node_creation_helper;

		NodeSocket* connection_in_progress_start = nullptr;
		bool box_select_active = false;
		FloatPos world_box_select_begin;
		FloatPos world_box_select_end;

		std::shared_ptr<Selection> selection;
		ZoomManager zoom;

		FloatPos view_center;

		FloatPos mouse_world_position;
		bool mouse_pan_active = false;

		bool node_move_active = false;
		bool node_move_did_something = false;

		bool should_push_undo_state = false;

		// Width and height of this view widget, in pixels on screen
		int widget_width;
		int widget_height;
	};

}