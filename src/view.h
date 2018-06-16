#pragma once

#include <memory>
#include <set>
#include <string>

#include "float_pos.h"
#include "selection.h"
#include "zoom.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class EditableGraph;
	class EditorNode;
	class NodeCreationHelper;
	class NodeSocket;
	class Selection;
	class ViewUIRequests;

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

		// Main 3 functions called from the main loop
		void set_mouse_position(FloatPos view_local_mouse_pos, int viewport_width, int viewport_height);
		void update();
		void draw(NVGcontext* draw_context);

		// Input
		void handle_mouse_button(int button, int action, int mods);
		void handle_key(int key, int scancode, int action, int mods);
		void handle_requests(ViewUIRequests& requests);

		// Undo
		bool needs_undo_push();

		// Selection
		std::weak_ptr<const Selection> get_const_selection() const;
		void clear_selection();

		// Misc
		std::string get_zoom_string() const;

	private:
		void begin_connection(NodeSocket* socket_begin);
		void complete_connection(NodeSocket* socket_end);
		void reroute_connection(NodeSocket* socket_end);
		void cancel_connection();

		void select_label(NodeSocket* label);

		std::set<EditorNode*> get_boxed_nodes();

		void node_move_begin();
		void node_move_end();

		void pan(int horizontal_ticks, int vertical_ticks);

		void box_select_begin();
		void box_select_end(SelectMode mode);

		void select_label_under_mouse();
		void deselect_label();

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
