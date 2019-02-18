#include "graph_editor.h"

#include "main_window.h"

CyclesShaderEditor::GraphEditor::GraphEditor()
{
	main_window = std::make_unique<EditorMainWindow>();
	const PathString default_font_path = Platform::get_pathstring("font");
	set_font_search_path(default_font_path);
}

CyclesShaderEditor::GraphEditor::~GraphEditor()
{
	// Empty destructor is needed so incomplete EditorMainWindow type can be used in the header
}

void CyclesShaderEditor::GraphEditor::set_font_search_path(const PathString& font_path)
{
	main_window->set_font_search_path(font_path);
}

bool CyclesShaderEditor::GraphEditor::create_window()
{
	return main_window->create_window();
}

bool CyclesShaderEditor::GraphEditor::run_window_loop_iteration()
{
	return main_window->run_window_loop_iteration();
}

void CyclesShaderEditor::GraphEditor::set_target_frame_rate(const double fps)
{
	main_window->set_target_frame_rate(fps);
}

void CyclesShaderEditor::GraphEditor::load_serialized_graph(const std::string& graph)
{
	main_window->load_serialized_graph(graph);
}

bool CyclesShaderEditor::GraphEditor::get_serialized_graph(std::string& graph)
{
	return main_window->get_serialized_output(graph);
}
