#pragma once

#include <list>

#include "common_enums.h"
#include "node_base.h"

namespace CyclesShaderEditor {
	
	// This class is used to store all data for a single graph that can be edited interactively
	class EditableGraph {
	public:
		EditableGraph(ShaderGraphType type);

		std::list<EditorNode*> nodes;
		std::list<NodeConnection> connections;

	private:
		void reset(ShaderGraphType type);

		void clear_nodes();
	};
}
