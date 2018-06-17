#include "node_interop_max.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::MaxTexmapShaderNode::MaxTexmapShaderNode(FloatPos position)
{
	world_pos = position;

	title = "3ds Max Texmap";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");
	const std::shared_ptr<NodeSocket> alpha_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Alpha", "alpha");

	sockets.push_back(color_output);
	sockets.push_back(alpha_output);

	const std::shared_ptr<NodeSocket> slot_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Int, "Slot", "slot");
	slot_input->value = new IntSocketValue(1, 1, 32);
	const std::shared_ptr<NodeSocket> autosize_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Boolean, "Auto-size", "autosize");
	autosize_input->value = new BoolSocketValue(true);
	const std::shared_ptr<NodeSocket> width_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Int, "Width", "width");
	width_input->value = new IntSocketValue(512, 1, 32768);
	const std::shared_ptr<NodeSocket> height_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Int, "Height", "height");
	height_input->value = new IntSocketValue(512, 1, 32768);
	const std::shared_ptr<NodeSocket> precision_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Precision", "precision");
	StringEnumSocketValue* precision_value = new StringEnumSocketValue();
	precision_value->enum_values.push_back(StringEnumPair("8-bit/Channel Int", "uchar"));
	precision_value->enum_values.push_back(StringEnumPair("32-bit/Channel Float", "float"));
	precision_value->set_from_internal_name("uchar");
	precision_input->value = precision_value;

	sockets.push_back(slot_input);
	sockets.push_back(autosize_input);
	sockets.push_back(width_input);
	sockets.push_back(height_input);
	sockets.push_back(precision_input);

	type = CyclesNodeType::MaxTex;
}
