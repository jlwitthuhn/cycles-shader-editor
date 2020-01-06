 #pragma once

#include <chrono>
#include <list>
#include <memory>
#include <string>

#include "ui_requests.h"
#include "util_platform.h"
#include "util_vector.h"
#include "undo.h"

namespace cse {

	class EditableGraph;
	class EditGraphView;
	class GlfwWindow;
	class GraphEditor;
	class NodeCreationHelper;
	class NodeEditorStatusBar;
	class NodeEditorSubwindow;
	class NodeEditorToolbar;
	class NvgContext;

	class EditorMainWindow {
	public:
		EditorMainWindow();
		~EditorMainWindow();

		void set_font_search_path(const PathString& font_path);

		bool create_window();
		bool run_window_loop_iteration();

		void set_target_frame_rate(double fps);

		void handle_mouse_button(int button, int action, int mods);
		void handle_key(int key, int scancode, int action, int mods);
		void handle_character(unsigned int codepoint);
		void handle_scroll(double xoffset, double yoffset);

		void load_serialized_graph(const std::string& graph_str);

		bool get_serialized_output(std::string& graph);

	private:
		void pre_draw();
		void draw();

		void swap_buffers();

		void service_requests();

		void update_mouse_position(Float2 screen_position);

		// Forwards input to any subwindow that wants to grab input
		// Returns true if a subwindow accepted the input, false otherwise
		bool forward_mouse_to_subwindow(int button, int action, int mods);
		bool forward_key_to_subwindow(int key, int scancode, int action, int mods);
		bool forward_character_to_subwindow(unsigned int codepoint);

		void update_serialized_state();
		void push_undo_state();

		void undo();
		void redo();

		void clear_graph(bool reset_undo);

		void do_output();

		void release_resources();

		std::shared_ptr<EditableGraph> main_graph;

		Float2 mouse_screen_pos;
		int window_width, window_height;

		std::string serialized_state;
		UndoStack undo_stack;

		std::shared_ptr<NodeCreationHelper> node_creation_helper;

		std::list<std::unique_ptr<NodeEditorSubwindow>> subwindows;

		std::unique_ptr<NodeEditorToolbar> toolbar;
		std::unique_ptr<NodeEditorStatusBar> status_bar;
		std::unique_ptr<EditGraphView> view;

		UIRequests requests;

		std::unique_ptr<GlfwWindow> glfw_window;
		std::unique_ptr<NvgContext> nvg_context;

		std::string serialized_output;
		bool serialized_output_updated = false;

		double target_frame_rate = 60.0;
		std::chrono::time_point<std::chrono::steady_clock> last_buffer_swap_time;

		PathString font_search_path;
	};

}
