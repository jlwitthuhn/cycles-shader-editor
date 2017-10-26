#include <graph_editor.h>

#include <iostream>

int main()
{
	CyclesShaderEditor::GraphEditor node_window;

	node_window.create_window();
	
	while (node_window.run_window_loop_iteration()) {
		// Check if data is ready
		if (node_window.output_updated) {
			std::cout << "Graph saved:\n" << node_window.serialized_output << std::endl;
			node_window.output_updated = false;
		}
	}
	return 0;
}

