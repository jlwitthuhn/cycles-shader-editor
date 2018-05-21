 #pragma once

#include <chrono>
#include <list>
#include <set>
#include <string>
#include <vector>

#include "node_base.h"
#include "point2.h"
#include "statusbar.h"
#include "toolbar.h"
#include "ui_requests.h"
#include "util_platform.h"
#include "undo.h"

struct GLFWwindow;
struct NVGcontext;

namespace CyclesShaderEditor {

	class EditGraphView;
	class EditorNode;
	class GraphEditor;
	class NodeEditorSubwindow;
	class NodeListSubwindow;
	class ParamEditorSubwindow;

	class EditorMainWindow {
	public:
		EditorMainWindow(GraphEditor* public_window);
		~EditorMainWindow();

		void set_font_search_path(const PathString& font_path);

		bool create_window();
		bool run_window_loop_iteration();

		void set_target_frame_rate(double fps);

		void handle_mouse_button(int button, int action, int mods);
		void handle_key(int key, int scancode, int action, int mods);
		void handle_character(unsigned int codepoint);
		void handle_scroll(double xoffset, double yoffset);

		void load_serialized_graph(const std::string& graph);

	private:
		void pre_draw();
		void draw();

		void swap_buffers();

		void service_requests();

		void update_mouse_position(Point2 screen_position);

		NodeEditorSubwindow* get_subwindow_under_mouse();
		void raise_subwindow(NodeEditorSubwindow* subwindow);

		void update_serialized_state();
		void push_undo_state();

		void undo();
		void redo();

		void clear_graph(bool reset_undo);

		void do_output();

		void release_resources();

		std::list<EditorNode*> nodes;
		std::list<NodeConnection> connections;

		Point2 mouse_screen_pos;
		int window_width, window_height;

		std::string serialized_state;
		UndoStack undo_stack;

		std::list<NodeEditorSubwindow*> subwindows;
		NodeListSubwindow* node_list_window = nullptr;
		ParamEditorSubwindow* param_editor_window = nullptr;

		NodeEditorToolbar* toolbar = nullptr;
		NodeEditorStatusBar* status_bar = nullptr;

		EditGraphView* view = nullptr;

		UIRequests requests;

		GLFWwindow* window = nullptr;
		NVGcontext* nvg_context = nullptr;

		GraphEditor* public_window = nullptr;

		double target_frame_rate = 60.0;
		std::chrono::time_point<std::chrono::steady_clock> last_buffer_swap_time;

		PathString font_search_path;
	};

}
