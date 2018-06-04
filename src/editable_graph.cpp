#include "editable_graph.h"

#include "node_outputs.h"

CyclesShaderEditor::EditableGraph::EditableGraph(const ShaderGraphType type)
{
	reset(type);
}

void CyclesShaderEditor::EditableGraph::reset(const ShaderGraphType type)
{
	connections.clear();
	clear_nodes();

	switch (type) {
	case ShaderGraphType::EMPTY:
		// Do nothing
		break;
	case ShaderGraphType::MATERIAL:
		{
			EditorNode* const output_node = new MaterialOutputNode(CyclesShaderEditor::FloatPos(0.0f, 0.0f));
			nodes.push_back(output_node);
		}
	}
}

// Remove this once nodes use smart pointers
void CyclesShaderEditor::EditableGraph::clear_nodes()
{
	for (const auto* this_node : nodes) {
		delete this_node;
	}
	nodes.clear();
}