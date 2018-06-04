#include "selection.h"

#include "node_base.h"

void CyclesShaderEditor::Selection::move_nodes(const FloatPos delta)
{
	for (auto* const this_node : nodes) {
		this_node->world_pos += delta;
	}
}
