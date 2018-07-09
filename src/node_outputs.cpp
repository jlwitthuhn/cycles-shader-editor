#include "node_outputs.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::MaterialOutputNode::MaterialOutputNode(FloatPos position)
{
	world_pos = position;

	title = "Material Output";

	const auto surface_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Surface", "surface");
	const auto volume_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Volume", "volume");
	const auto displacement_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Displacement", "displacement");

	sockets.push_back(surface_input);
	sockets.push_back(volume_input);
	sockets.push_back(displacement_input);

	type = CyclesNodeType::MaterialOutput;

	// This node being added should not trigger an update to undo state
	changed = false;
}

bool CyclesShaderEditor::MaterialOutputNode::can_be_deleted()
{
	return false;
}
