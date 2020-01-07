#include "node_outputs.h"

#include <memory>
#include <vector>

#include "output.h"
#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::MaterialOutputNode::MaterialOutputNode(const Float2 position) : EditableNode(NodeCategory::OUTPUT, CyclesNodeType::MaterialOutput, "Material Output")
{
	world_pos = position;

	const auto surface_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Surface", "surface");
	const auto volume_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Volume", "volume");
	const auto displacement_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Displacement", "displacement");

	sockets.push_back(surface_input);
	sockets.push_back(volume_input);
	sockets.push_back(displacement_input);

	// This node being added should not trigger an update to undo state
	changed = false;
}

bool cse::MaterialOutputNode::can_be_deleted()
{
	return false;
}
