#include "node_outputs.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::MaterialOutputNode::MaterialOutputNode(FloatPos position)
{
	world_pos = position;

	title = "Material Output";

	NodeSocket* surface_input = new NodeSocket(this, SocketIOType::Input, SocketType::Closure, "Surface", "surface");
	NodeSocket* volume_input = new NodeSocket(this, SocketIOType::Input, SocketType::Closure, "Volume", "volume");
	NodeSocket* displacement_input = new NodeSocket(this, SocketIOType::Input, SocketType::Float, "Displacement", "displacement");

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