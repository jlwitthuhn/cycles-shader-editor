#include "node_colors.h"

CyclesShaderEditor::MixRGBNode::MixRGBNode(Point2 position)
{
	world_pos = position;

	title = "Mix RGB";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Linear Light", "linear_light"));
	type_value->enum_values.push_back(StringEnumPair("Soft Light", "soft_light"));
	type_value->enum_values.push_back(StringEnumPair("Color", "color"));
	type_value->enum_values.push_back(StringEnumPair("Value", "value"));
	type_value->enum_values.push_back(StringEnumPair("Saturation", "saturation"));
	type_value->enum_values.push_back(StringEnumPair("Hue", "hue"));
	type_value->enum_values.push_back(StringEnumPair("Burn", "burn"));
	type_value->enum_values.push_back(StringEnumPair("Dodge", "dodge"));
	type_value->enum_values.push_back(StringEnumPair("Overlay", "overlay"));
	type_value->enum_values.push_back(StringEnumPair("Lighten", "lighten"));
	type_value->enum_values.push_back(StringEnumPair("Darken", "darken"));
	type_value->enum_values.push_back(StringEnumPair("Difference", "difference"));
	type_value->enum_values.push_back(StringEnumPair("Divide", "divide"));
	type_value->enum_values.push_back(StringEnumPair("Screen", "screen"));
	type_value->enum_values.push_back(StringEnumPair("Subtract", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Multiply", "multiply"));
	type_value->enum_values.push_back(StringEnumPair("Add", "add"));
	type_value->enum_values.push_back(StringEnumPair("Mix", "mix"));
	type_value->set_from_internal_name("mix");
	type_input->value = type_value;
	NodeSocket* clamp_input = new NodeSocket(this, SocketInOut::Input, SocketType::Boolean, "Clamp", "use_clamp");
	clamp_input->value = new BoolSocketValue(false);
	NodeSocket* fac_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	NodeSocket* color1_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color1", "color1");
	color1_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	NodeSocket* color2_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color2", "color2");
	color2_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(type_input);
	sockets.push_back(clamp_input);
	sockets.push_back(fac_input);
	sockets.push_back(color1_input);
	sockets.push_back(color2_input);

	type = CyclesNodeType::MixRGB;
}

CyclesShaderEditor::InvertNode::InvertNode(Point2 position)
{
	world_pos = position;

	title = "Invert";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* fac_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(fac_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::Invert;
}

CyclesShaderEditor::LightFalloffNode::LightFalloffNode(Point2 position)
{
	world_pos = position;

	title = "Light Falloff";

	NodeSocket* quadratic_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Quadratic", "quadratic");
	NodeSocket* linear_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Linear", "linear");
	NodeSocket* constant_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Constant", "constant");

	sockets.push_back(quadratic_output);
	sockets.push_back(linear_output);
	sockets.push_back(constant_output);

	NodeSocket* strength_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(100.0f, 0.0f, 10000.0f);
	NodeSocket* smooth_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Smooth", "smooth");
	smooth_input->value = new FloatSocketValue(0.0f, 0.0f, 1000.0f);

	sockets.push_back(strength_input);
	sockets.push_back(smooth_input);

	type = CyclesNodeType::LightFalloff;
}

CyclesShaderEditor::HSVNode::HSVNode(Point2 position)
{
	world_pos = position;

	title = "HSV";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* hue_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Hue", "hue");
	hue_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	NodeSocket* sat_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Saturation", "saturation");
	sat_input->value = new FloatSocketValue(1.0f, 0.0f, 2.0f);
	NodeSocket* val_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Value", "value");
	val_input->value = new FloatSocketValue(1.0f, 0.0f, 2.0f);
	NodeSocket* fac_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(hue_input);
	sockets.push_back(sat_input);
	sockets.push_back(val_input);
	sockets.push_back(fac_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::HSV;
}

CyclesShaderEditor::GammaNode::GammaNode(Point2 position)
{
	world_pos = position;

	title = "Gamma";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	NodeSocket* gamma_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Gamma", "gamma");
	gamma_input->value = new FloatSocketValue(1.0f, 0.001f, 10.0f);

	sockets.push_back(color_input);
	sockets.push_back(gamma_input);

	type = CyclesNodeType::Gamma;
}

CyclesShaderEditor::BrightnessContrastNode::BrightnessContrastNode(Point2 position)
{
	world_pos = position;

	title = "Bright/Contrast";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	NodeSocket* bright_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Bright", "bright");
	bright_input->value = new FloatSocketValue(0.0f, -100.0f, 100.0f);
	NodeSocket* cont_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Contrast", "contrast");
	cont_input->value = new FloatSocketValue(0.0f, -100.0f, 100.0f);

	sockets.push_back(color_input);
	sockets.push_back(bright_input);
	sockets.push_back(cont_input);

	type = CyclesNodeType::BrightnessContrast;
}