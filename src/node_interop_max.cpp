#include "node_interop_max.h"

CyclesShaderEditor::MaxTexmapShaderNode::MaxTexmapShaderNode(FloatPos position)
{
	world_pos = position;

	title = "3ds Max Texmap";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* alpha_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Alpha", "alpha");

	sockets.push_back(color_output);
	sockets.push_back(alpha_output);

	NodeSocket* slot_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Slot", "slot");
	slot_input->value = new IntSocketValue(1, 1, 32);
	NodeSocket* autosize_input = new NodeSocket(this, SocketInOut::Input, SocketType::Boolean, "Auto-size", "autosize");
	autosize_input->value = new BoolSocketValue(true);
	NodeSocket* width_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Width", "width");
	width_input->value = new IntSocketValue(512, 1, 32768);
	NodeSocket* height_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Height", "height");
	height_input->value = new IntSocketValue(512, 1, 32768);
	NodeSocket* precision_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Precision", "precision");
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
