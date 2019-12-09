#include "node_vector.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

cse::BumpNode::BumpNode(const FloatPos position) : EditableNode(NodeCategory::VECTOR, CyclesNodeType::Bump, "Bump")
{
	world_pos = position;

	const auto normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(normal_output);

	const auto invert_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Invert", "invert");
	invert_input->value = std::make_shared<BoolSocketValue>(false);
	const auto strength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Strength", "strength");
	strength_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);
	const auto distance_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Distance", "distance");
	distance_input->value = std::make_shared<FloatSocketValue>(0.1f, 0.0f, 1000.0f);
	const auto height_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Height", "height");
	height_input->selectable = false;
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(invert_input);
	sockets.push_back(strength_input);
	sockets.push_back(distance_input);
	sockets.push_back(height_input);
	sockets.push_back(normal_input);
}

cse::NormalMapNode::NormalMapNode(const FloatPos position) : EditableNode(NodeCategory::VECTOR, CyclesNodeType::NormalMap, "Normal Map")
{
	world_pos = position;

	const auto normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(normal_output);

	const auto space_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Space", "space");
	const auto space_value = std::make_shared<StringEnumSocketValue>();
	space_value->enum_values.push_back(StringEnumPair("Tangent", "tangent"));
	space_value->enum_values.push_back(StringEnumPair("Object", "object"));
	space_value->enum_values.push_back(StringEnumPair("World", "world"));
	space_value->set_from_internal_name("tangent");
	space_input->value = space_value;
	const auto strength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Strength", "strength");
	strength_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 10.0f);
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 1.0f);

	sockets.push_back(space_input);
	sockets.push_back(strength_input);
	sockets.push_back(color_input);
}

cse::VectorTransformNode::VectorTransformNode(const FloatPos position) : EditableNode(NodeCategory::VECTOR, CyclesNodeType::VectorTransform, "Vector Transform")
{
	world_pos = position;

	const auto vector_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Vector", "vector");

	sockets.push_back(vector_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Vector", "vector"));
	type_value->enum_values.push_back(StringEnumPair("Point", "point"));
	type_value->enum_values.push_back(StringEnumPair("Normal", "normal"));
	type_value->set_from_internal_name("vector");
	type_input->value = type_value;

	const auto convert_from_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Convert From", "convert_from");
	const auto convert_from_value = std::make_shared<StringEnumSocketValue>();
	convert_from_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_from_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_from_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_from_value->set_from_internal_name("world");
	convert_from_input->value = convert_from_value;

	const auto convert_to_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Convert To", "convert_to");
	const auto convert_to_value = std::make_shared<StringEnumSocketValue>();
	convert_to_value->enum_values.push_back(StringEnumPair("Camera", "camera"));
	convert_to_value->enum_values.push_back(StringEnumPair("Object", "object"));
	convert_to_value->enum_values.push_back(StringEnumPair("World", "world"));
	convert_to_value->set_from_internal_name("object");
	convert_to_input->value = convert_to_value;

	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	vector_input->value = std::make_shared<Float3SocketValue>(0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f, 0.0f, -100000.0f, 100000.0f);
	vector_input->selectable = true;

	sockets.push_back(type_input);
	sockets.push_back(convert_from_input);
	sockets.push_back(convert_to_input);
	sockets.push_back(vector_input);
}
