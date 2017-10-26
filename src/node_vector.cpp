#include "node_vector.h"

CyclesShaderEditor::BumpNode::BumpNode(Point2 position)
{
	world_pos = position;

	title = "Bump";

	NodeSocket* normal_output = new NodeSocket(this, SocketInOut::Output, SocketType::Normal, "Normal", "normal");

	sockets.push_back(normal_output);

	NodeSocket* invert_input = new NodeSocket(this, SocketInOut::Input, SocketType::Boolean, "Invert", "invert");
	invert_input->value = new BoolSocketValue(false);
	NodeSocket* strength_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	NodeSocket* distance_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Distance", "distance");
	distance_input->value = new FloatSocketValue(0.1f, 0.0f, 1000.0f);
	NodeSocket* height_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Height", "height");
	height_input->selectable = false;
	NodeSocket* normal_input = new NodeSocket(this, SocketInOut::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(invert_input);
	sockets.push_back(strength_input);
	sockets.push_back(distance_input);
	sockets.push_back(height_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::Bump;
}

CyclesShaderEditor::NormalMapNode::NormalMapNode(Point2 position)
{
	world_pos = position;

	title = "Normal Map";

	NodeSocket* normal_output = new NodeSocket(this, SocketInOut::Output, SocketType::Normal, "Normal", "normal");

	sockets.push_back(normal_output);

	NodeSocket* space_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Space", "space");
	StringEnumSocketValue* space_value = new StringEnumSocketValue();
	space_value->enum_values.push_back(StringEnumPair("Tangent", "tangent"));
	space_value->enum_values.push_back(StringEnumPair("Object", "object"));
	space_value->enum_values.push_back(StringEnumPair("World", "world"));
	space_value->set_from_internal_name("tangent");
	space_input->value = space_value;
	NodeSocket* strength_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(1.0f, 0.0f, 10.0f);
	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(0.5f, 0.5f, 1.0f);

	sockets.push_back(space_input);
	sockets.push_back(strength_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::NormalMap;
}