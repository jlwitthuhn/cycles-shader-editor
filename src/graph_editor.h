#pragma once

#include <memory>
#include <string>

#include "util_platform.h"

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

		bool get_serialized_graph(std::string& graph);

	private:
		std::unique_ptr<EditorMainWindow> main_window;
	};

}
