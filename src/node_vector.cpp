#include "node_vector.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::BumpNode::BumpNode(FloatPos position)
{
	world_pos = position;

	title = "Bump";

	const std::shared_ptr<NodeSocket> normal_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Normal, "Normal", "normal");

	sockets.push_back(normal_output);

	const std::shared_ptr<NodeSocket> invert_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Boolean, "Invert", "invert");
	invert_input->value = new BoolSocketValue(false);
	const std::shared_ptr<NodeSocket> strength_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> distance_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Distance", "distance");
	distance_input->value = new FloatSocketValue(0.1f, 0.0f, 1000.0f);
	const std::shared_ptr<NodeSocket> height_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Height", "height");
	height_input->selectable = false;
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

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

	const std::shared_ptr<NodeSocket> normal_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Normal, "Normal", "normal");

	sockets.push_back(normal_output);

	const std::shared_ptr<NodeSocket> space_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Space", "space");
	StringEnumSocketValue* space_value = new StringEnumSocketValue();
	space_value->enum_values.push_back(StringEnumPair("Tangent", "tangent"));
	space_value->enum_values.push_back(StringEnumPair("Object", "object"));
	space_value->enum_values.push_back(StringEnumPair("World", "world"));
	space_value->set_from_internal_name("tangent");
	space_input->value = space_value;
	const std::shared_ptr<NodeSocket> strength_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(1.0f, 0.0f, 10.0f);
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
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

	const std::shared_ptr<NodeSocket> vector_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Vector", "vector");

	sockets.push_back(vector_output);

	const std::shared_ptr<NodeSocket> type_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Vector", "vector"));
	type_value->enum_values.push_back(StringEnumPair("Point", "point"));
	type_value->enum_values.push_back(StringEnumPair("Normal", "normal"));
	type_value->set_from_internal_name("vector");
	type_input->value = type_value;

	const std::shared_ptr<NodeSocket> convert_from_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Convert From", "convert_from");
	StringEnumSocketValue* convert_from_value = new StringEnumSocketValue();
	convert_from_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_from_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_from_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_from_value->set_from_internal_name("world");
	convert_from_input->value = convert_from_value;

	const std::shared_ptr<NodeSocket> convert_to_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Convert To", "convert_to");
	StringEnumSocketValue* convert_to_value = new StringEnumSocketValue();
	convert_to_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_to_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_to_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_to_value->set_from_internal_name("object");
	convert_to_input->value = convert_to_value;

	const std::shared_ptr<NodeSocket> vector_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Vector", "vector");
	vector_input->value = new Float3SocketValue(0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f);
	vector_input->selectable = true;

	sockets.push_back(type_input);
	sockets.push_back(convert_from_input);
	sockets.push_back(convert_to_input);
	sockets.push_back(vector_input);

	type = CyclesNodeType::VectorTransform;
}
