#include "selection.h"

#include "float_pos.h"
#include "node_base.h"

void cse::Selection::move_nodes(const FloatPos delta)
{
	for (const auto weak_node : nodes) {
		if (const auto this_node = weak_node.lock()) {
			this_node->world_pos += delta;
		}
	}
}

void cse::Selection::modify_selection(SelectMode mode, std::weak_ptr<EditableNode> node)
{
	if (node.lock().get() == nullptr) {
		return;
	}

	switch (mode) {
		case SelectMode::NORMAL:
		{
			if (nodes.count(node) > 0) {
				// This node is already selected, do not change selection
			}
			else {
				// This node is not selected, now select this node exclusively
				nodes.clear();
				nodes.insert(node);
			}
			break;
		}
		case SelectMode::ADD:
		{
			nodes.insert(node);
			break;
		}
		case SelectMode::TOGGLE:
		{
			if (nodes.count(node) > 0) {
				nodes.erase(node);
			}
			else {
				nodes.insert(node);
			}
			break;
		}
	}
}

void cse::Selection::clear()
{
	nodes.clear();
	socket = std::weak_ptr<NodeSocket>();
}