#pragma once

#include <list>
#include <set>

#include "float_pos.h"
#include "node_base.h"
#include "zoom.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	enum class SelectMode {
		EXCLUSIVE,
		ADD,
		TOGGLE,
		NONE,
	};

	class EditGraphView {
	public:
		EditGraphView(
			std::list<EditorNode*>& nodes,
			std::list<NodeConnection>& connections
		);

		// Below functions query the state of the view
		EditorNode* get_node_under_mouse();
		NodeSocket* get_socket_label_under_mouse();
		NodeSocket* get_selected_socket_label();
		NodeSocket* get_socket_under_mouse();
		FloatPos get_mouse_world_position();
		bool is_node_under_mouse_selected();
		std::string get_zoom_string();

		// Main 2 functions called from the main loop
		void update(FloatPos view_local_mouse_pos, int viewport_width, int viewport_height);
		void draw(NVGcontext* draw_context);

		// Functions to manipulate view state
		void add_node_at_mouse(EditorNode* node);
		void begin_connection_under_mouse();
		void cancel_connection();
		void complete_connection_at_mouse();
		void clear_input_socket_under_mouse();
		void clear_node_selection();

		void delete_selected_nodes();
		void delete_node(EditorNode* const node);

		void node_move_begin();
		void node_move_end();

		void move_left();
		void move_right();
		void move_up();
		void move_down();

		void zoom_in();
		void zoom_out();

		void mouse_move_begin();
		void mouse_move_end();

		void box_select_begin();
		void box_select_end(SelectMode mode);
		
		void select_label_under_mouse();
		void deselect_label();

		void raise_node_under_mouse(SelectMode mode);

	private:
		void select_label(NodeSocket* label);

		void raise_node(EditorNode* node, SelectMode mode);

		std::set<EditorNode*> get_boxed_nodes();


		// Graph state
		std::list<EditorNode*>& nodes;
		std::list<NodeConnection>& connections;

		// View state
		std::set<EditorNode*> selected_nodes;
		NodeSocket* connection_in_progress_start = nullptr;
		NodeSocket* selected_label = nullptr;
		bool box_select_active = false;
		FloatPos world_box_select_begin;
		FloatPos world_box_select_end;

		FloatPos mouse_world_position;

		bool mouse_move_active = false;

		ZoomManager zoom;

		FloatPos view_center;

		float border_left = 0.0f;
		float border_right = 1.0f;
		float border_top = 0.0f;
		float border_bottom = 1.0f;
	};

}