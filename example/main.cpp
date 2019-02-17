#include <graph_editor.h>

#include <iostream>
#include <string>

int main()
{
	CyclesShaderEditor::GraphEditor node_window;

	node_window.create_window();
	
	while (node_window.run_window_loop_iteration()) {
		// Check if data is ready
		std::string graph;
		if (node_window.get_serialized_graph(graph)) {
			std::cout << "Graph saved:\n" << graph << std::endl;
		}
	}
	return 0;
}
