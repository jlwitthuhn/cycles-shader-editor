#pragma once

#include "util_platform.h"

#include <string>

namespace CyclesShaderEditor {

	class EditorMainWindow;

	class GraphEditor {
	public:
		GraphEditor();
		~GraphEditor();

		void set_font_search_path(const PathString& font_path);

		bool create_window();
		bool run_window_loop_iteration();

		void set_target_frame_rate(double fps);

		void load_serialized_graph(const std::string& graph);

		std::string serialized_output;
		bool output_updated = false;

	private:
		EditorMainWindow* main_window = nullptr;
	};

}
