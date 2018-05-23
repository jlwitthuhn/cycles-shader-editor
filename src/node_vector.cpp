#include "node_vector.h"

CyclesShaderEditor::BumpNode::BumpNode(FloatPos position)
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

CyclesShaderEditor::NormalMapNode::NormalMapNode(FloatPos position)
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

CyclesShaderEditor::VectorTransformNode::VectorTransformNode(FloatPos position)
{
	world_pos = position;

	title = "Vector Transform";

	NodeSocket* vector_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Vector", "vector");

	sockets.push_back(vector_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Vector", "vector"));
	type_value->enum_values.push_back(StringEnumPair("Point", "point"));
	type_value->enum_values.push_back(StringEnumPair("Normal", "normal"));
	type_value->set_from_internal_name("vector");
	type_input->value = type_value;

	NodeSocket* convert_from_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Convert From", "convert_from");
	StringEnumSocketValue* convert_from_value = new StringEnumSocketValue();
	convert_from_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_from_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_from_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_from_value->set_from_internal_name("world");
	convert_from_input->value = convert_from_value;

	NodeSocket* convert_to_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Convert To", "convert_to");
	StringEnumSocketValue* convert_to_value = new StringEnumSocketValue();
	convert_to_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_to_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_to_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_to_value->set_from_internal_name("object");
	convert_to_input->value = convert_to_value;

	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	vector_input->value = new Float3SocketValue(0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f);
	vector_input->selectable = true;

	sockets.push_back(type_input);
	sockets.push_back(convert_from_input);
	sockets.push_back(convert_to_input);
	sockets.push_back(vector_input);

	type = CyclesNodeType::VectorTransform;
}
