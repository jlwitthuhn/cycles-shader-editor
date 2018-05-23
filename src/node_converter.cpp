#include "node_converter.h"

CyclesShaderEditor::BlackbodyNode::BlackbodyNode(FloatPos position)
{
	world_pos = position;

	title = "Blackbody";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* temp_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Temperature", "temperature");
	temp_input->value = new FloatSocketValue(1500.0f, 800.0f, 20000.0f);

	sockets.push_back(temp_input);

	type = CyclesNodeType::Blackbody;

	content_width = 164.0f;
}

CyclesShaderEditor::CombineHSVNode::CombineHSVNode(FloatPos position)
{
	world_pos = position;

	title = "Combine HSV";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* h_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "H", "h");
	h_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	NodeSocket* s_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "S", "s");
	s_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	NodeSocket* v_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "V", "v");
	v_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);

	sockets.push_back(h_input);
	sockets.push_back(s_input);
	sockets.push_back(v_input);

	type = CyclesNodeType::CombineHSV;
}

CyclesShaderEditor::CombineRGBNode::CombineRGBNode(FloatPos position)
{
	world_pos = position;

	title = "Combine RGB";

	NodeSocket* image_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Image", "image");

	sockets.push_back(image_output);

	NodeSocket* r_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "R", "r");
	r_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	NodeSocket* g_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "G", "g");
	g_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	NodeSocket* b_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "B", "b");
	b_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);

	sockets.push_back(r_input);
	sockets.push_back(g_input);
	sockets.push_back(b_input);

	type = CyclesNodeType::CombineRGB;
}

CyclesShaderEditor::CombineXYZNode::CombineXYZNode(FloatPos position)
{
	world_pos = position;

	title = "Combine XYZ";

	NodeSocket* vec_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Vector", "vector");

	sockets.push_back(vec_output);

	NodeSocket* x_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "X", "x");
	x_input->value = new FloatSocketValue(0.0f, -100000.0f, 100000.0f);
	NodeSocket* y_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Y", "y");
	y_input->value = new FloatSocketValue(0.0f, -100000.0f, 100000.0f);
	NodeSocket* z_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Z", "z");
	z_input->value = new FloatSocketValue(0.0f, -100000.0f, 100000.0f);

	sockets.push_back(x_input);
	sockets.push_back(y_input);
	sockets.push_back(z_input);

	type = CyclesNodeType::CombineXYZ;
}

CyclesShaderEditor::MathNode::MathNode(FloatPos position)
{
	world_pos = position;

	title = "Math";

	NodeSocket* value_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Value", "value");

	sockets.push_back(value_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Add", "add"));
	type_value->enum_values.push_back(StringEnumPair("Subtract", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Multiply", "multiply"));
	type_value->enum_values.push_back(StringEnumPair("Divide", "divide"));
	type_value->enum_values.push_back(StringEnumPair("Sine", "sine"));
	type_value->enum_values.push_back(StringEnumPair("Cosine", "cosine"));
	type_value->enum_values.push_back(StringEnumPair("Tangent", "tangent"));
	type_value->enum_values.push_back(StringEnumPair("Arcsine", "arcsine"));
	type_value->enum_values.push_back(StringEnumPair("Arccosine", "arccosine"));
	type_value->enum_values.push_back(StringEnumPair("Arctangent", "arctangent"));
	type_value->enum_values.push_back(StringEnumPair("Power", "power"));
	type_value->enum_values.push_back(StringEnumPair("Logarithm", "logarithm"));
	type_value->enum_values.push_back(StringEnumPair("Minimum", "minimum"));
	type_value->enum_values.push_back(StringEnumPair("Maximum", "maximum"));
	type_value->enum_values.push_back(StringEnumPair("Round", "round"));
	type_value->enum_values.push_back(StringEnumPair("Less Than", "less_than"));
	type_value->enum_values.push_back(StringEnumPair("Greater Than", "greater_than"));
	type_value->enum_values.push_back(StringEnumPair("Modulo", "modulo"));
	type_value->enum_values.push_back(StringEnumPair("Absolute", "absolute"));
	type_value->set_from_internal_name("add");
	type_input->value = type_value;
	NodeSocket* clamp_input = new NodeSocket(this, SocketInOut::Input, SocketType::Boolean, "Clamp", "use_clamp");
	clamp_input->value = new BoolSocketValue(false);
	NodeSocket* value1_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Value1", "value1");
	value1_input->value = new FloatSocketValue(0.0f, -100000.0f, 100000.0f);
	NodeSocket* value2_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Value2", "value2");
	value2_input->value = new FloatSocketValue(0.0f, -100000.0f, 100000.0f);

	sockets.push_back(type_input);
	sockets.push_back(clamp_input);
	sockets.push_back(value1_input);
	sockets.push_back(value2_input);

	type = CyclesNodeType::Math;
}

CyclesShaderEditor::RGBToBWNode::RGBToBWNode(FloatPos position)
{
	world_pos = position;

	title = "RGB to BW";

	NodeSocket* val_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Val", "val");

	sockets.push_back(val_output);

	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);

	type = CyclesNodeType::RGBtoBW;
}

CyclesShaderEditor::SeparateHSVNode::SeparateHSVNode(FloatPos position)
{
	world_pos = position;

	title = "Separate HSV";

	NodeSocket* h_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "H", "h");
	NodeSocket* s_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "S", "s");
	NodeSocket* v_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "V", "v");

	sockets.push_back(h_output);
	sockets.push_back(s_output);
	sockets.push_back(v_output);

	NodeSocket* color_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);

	type = CyclesNodeType::SeparateHSV;
}

CyclesShaderEditor::SeparateRGBNode::SeparateRGBNode(FloatPos position)
{
	world_pos = position;

	title = "Separate RGB";

	NodeSocket* r_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "R", "r");
	NodeSocket* g_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "G", "g");
	NodeSocket* b_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "B", "b");

	sockets.push_back(r_output);
	sockets.push_back(g_output);
	sockets.push_back(b_output);

	NodeSocket* image_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Image", "image");
	image_input->value = new ColorSocketValue(0.5f, 0.5f, 0.5f);

	sockets.push_back(image_input);

	type = CyclesNodeType::SeparateRGB;
}

CyclesShaderEditor::SeparateXYZNode::SeparateXYZNode(FloatPos position)
{
	world_pos = position;

	title = "Separate XYZ";

	NodeSocket* x_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "X", "x");
	NodeSocket* y_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Y", "y");
	NodeSocket* z_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Z", "z");

	sockets.push_back(x_output);
	sockets.push_back(y_output);
	sockets.push_back(z_output);

	NodeSocket* vec_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	vec_input->value = new Float3SocketValue(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
	vec_input->selectable = true;

	sockets.push_back(vec_input);

	type = CyclesNodeType::SeparateXYZ;
}

CyclesShaderEditor::VectorMathNode::VectorMathNode(FloatPos position)
{
	world_pos = position;

	title = "Vector Math";

	NodeSocket* vector_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Vector", "vector");
	NodeSocket* value_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Value", "value");

	sockets.push_back(vector_output);
	sockets.push_back(value_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Add", "add"));
	type_value->enum_values.push_back(StringEnumPair("Subtract", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Average", "average"));
	type_value->enum_values.push_back(StringEnumPair("Dot Product", "dot_product"));
	type_value->enum_values.push_back(StringEnumPair("Cross Product", "cross_product"));
	type_value->enum_values.push_back(StringEnumPair("Normalize", "normalize"));
	type_value->set_from_internal_name("add");
	type_input->value = type_value;
	NodeSocket* vector1_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector1", "vector1");
	vector1_input->value = new Float3SocketValue(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
	vector1_input->selectable = true;
	NodeSocket* vector2_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector2", "vector2");
	vector2_input->value = new Float3SocketValue(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
	vector2_input->selectable = true;

	sockets.push_back(type_input);
	sockets.push_back(vector1_input);
	sockets.push_back(vector2_input);

	type = CyclesNodeType::VectorMath;
}

CyclesShaderEditor::WavelengthNode::WavelengthNode(FloatPos position)
{
	world_pos = position;

	title = "Wavelength";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* wavelength_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Wavelength", "wavelength");
	wavelength_input->value = new FloatSocketValue(500.0f, 380.0f, 780.0f);

	sockets.push_back(wavelength_input);

	type = CyclesNodeType::Wavelength;
}
