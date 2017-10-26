#include "node_outputs.h"

CyclesShaderEditor::MaterialOutputNode::MaterialOutputNode(Point2 position)
{
	world_pos = position;

	title = "Material Output";

	NodeSocket* surface_input = new NodeSocket(this, SocketInOut::Input, SocketType::Closure, "Surface", "surface");
	NodeSocket* volume_input = new NodeSocket(this, SocketInOut::Input, SocketType::Closure, "Volume", "volume");

	sockets.push_back(surface_input);
	sockets.push_back(volume_input);

	type = CyclesNodeType::MaterialOutput;

	content_width = 140.0f;
	content_height = 100.0f;

	node_moving = false;

	// This node being added should not trigger an update to undo state
	changed = false;
}

bool CyclesShaderEditor::MaterialOutputNode::can_be_deleted()
{
	return false;
}