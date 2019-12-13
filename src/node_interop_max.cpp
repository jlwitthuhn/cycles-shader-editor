#include "node_interop_max.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

cse::MaxTexmapShaderNode::MaxTexmapShaderNode(const FloatPos position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::MaxTex, "3ds Max Texmap")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto alpha_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Alpha", "alpha");

	sockets.push_back(color_output);
	sockets.push_back(alpha_output);

	const auto slot_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Slot", "slot");
	slot_input->value = std::make_shared<IntSocketValue>(1, 1, 32);
	const auto autosize_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Auto-size", "autosize");
	autosize_input->value = std::make_shared<BoolSocketValue>(true);
	const auto width_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Width", "width");
	width_input->value = std::make_shared<IntSocketValue>(512, 1, 32768);
	const auto height_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Height", "height");
	height_input->value = std::make_shared<IntSocketValue>(512, 1, 32768);
	const auto precision_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Precision", "precision");
	const auto precision_value = std::make_shared<StringEnumSocketValue>();
	precision_value->enum_values.push_back(StringEnumPair("8-bit/Channel Int", "uchar"));
	precision_value->enum_values.push_back(StringEnumPair("32-bit/Channel Float", "float"));
	precision_value->set_from_internal_name("uchar");
	precision_input->value = precision_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");

	sockets.push_back(slot_input);
	sockets.push_back(autosize_input);
	sockets.push_back(width_input);
	sockets.push_back(height_input);
	sockets.push_back(precision_input);
	sockets.push_back(vector_input);
}
