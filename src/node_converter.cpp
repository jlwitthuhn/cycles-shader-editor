#include "node_converter.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::BlackbodyNode::BlackbodyNode(FloatPos position)
{
	world_pos = position;

	title = "Blackbody";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto temp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Temperature", "temperature");
	temp_input->value = std::make_shared<FloatSocketValue>(1500.0f, 800.0f, 20000.0f);

	sockets.push_back(temp_input);

	type = CyclesNodeType::Blackbody;

	content_width = 164.0f;
}

CyclesShaderEditor::CombineHSVNode::CombineHSVNode(FloatPos position)
{
	world_pos = position;

	title = "Combine HSV";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto h_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "H", "h");
	h_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto s_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "S", "s");
	s_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto v_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "V", "v");
	v_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	sockets.push_back(h_input);
	sockets.push_back(s_input);
	sockets.push_back(v_input);

	type = CyclesNodeType::CombineHSV;
}

CyclesShaderEditor::CombineRGBNode::CombineRGBNode(FloatPos position)
{
	world_pos = position;

	title = "Combine RGB";

	const auto image_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Image", "image");

	sockets.push_back(image_output);

	const auto r_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "R", "r");
	r_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto g_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "G", "g");
	g_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto b_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "B", "b");
	b_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	sockets.push_back(r_input);
	sockets.push_back(g_input);
	sockets.push_back(b_input);

	type = CyclesNodeType::CombineRGB;
}

CyclesShaderEditor::CombineXYZNode::CombineXYZNode(FloatPos position)
{
	world_pos = position;

	title = "Combine XYZ";

	const auto vec_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Vector", "vector");

	sockets.push_back(vec_output);

	const auto x_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "X", "x");
	x_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);
	const auto y_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Y", "y");
	y_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);
	const auto z_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Z", "z");
	z_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);

	sockets.push_back(x_input);
	sockets.push_back(y_input);
	sockets.push_back(z_input);

	type = CyclesNodeType::CombineXYZ;
}

CyclesShaderEditor::MathNode::MathNode(FloatPos position)
{
	world_pos = position;

	title = "Math";

	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(value_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
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
	const auto clamp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Clamp", "use_clamp");
	clamp_input->value = std::make_shared<BoolSocketValue>(false);
	const auto value1_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value1", "value1");
	value1_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);
	const auto value2_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value2", "value2");
	value2_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);

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

	const auto val_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Val", "val");

	sockets.push_back(val_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);

	type = CyclesNodeType::RGBtoBW;
}

CyclesShaderEditor::SeparateHSVNode::SeparateHSVNode(FloatPos position)
{
	world_pos = position;

	title = "Separate HSV";

	const auto h_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "H", "h");
	const auto s_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "S", "s");
	const auto v_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "V", "v");

	sockets.push_back(h_output);
	sockets.push_back(s_output);
	sockets.push_back(v_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);

	type = CyclesNodeType::SeparateHSV;
}

CyclesShaderEditor::SeparateRGBNode::SeparateRGBNode(FloatPos position)
{
	world_pos = position;

	title = "Separate RGB";

	const auto r_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "R", "r");
	const auto g_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "G", "g");
	const auto b_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "B", "b");

	sockets.push_back(r_output);
	sockets.push_back(g_output);
	sockets.push_back(b_output);

	const auto image_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Image", "image");
	image_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(image_input);

	type = CyclesNodeType::SeparateRGB;
}

CyclesShaderEditor::SeparateXYZNode::SeparateXYZNode(FloatPos position)
{
	world_pos = position;

	title = "Separate XYZ";

	const auto x_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "X", "x");
	const auto y_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Y", "y");
	const auto z_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Z", "z");

	sockets.push_back(x_output);
	sockets.push_back(y_output);
	sockets.push_back(z_output);

	const auto vec_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	vec_input->value = std::make_shared<Float3SocketValue>(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
	vec_input->selectable = true;

	sockets.push_back(vec_input);

	type = CyclesNodeType::SeparateXYZ;
}

CyclesShaderEditor::VectorMathNode::VectorMathNode(FloatPos position)
{
	world_pos = position;

	title = "Vector Math";

	const auto vector_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Vector", "vector");
	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(vector_output);
	sockets.push_back(value_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Add", "add"));
	type_value->enum_values.push_back(StringEnumPair("Subtract", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Average", "average"));
	type_value->enum_values.push_back(StringEnumPair("Dot Product", "dot_product"));
	type_value->enum_values.push_back(StringEnumPair("Cross Product", "cross_product"));
	type_value->enum_values.push_back(StringEnumPair("Normalize", "normalize"));
	type_value->set_from_internal_name("add");
	type_input->value = type_value;
	const auto vector1_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector1", "vector1");
	vector1_input->value = std::make_shared<Float3SocketValue>(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
	vector1_input->selectable = true;
	const auto vector2_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector2", "vector2");
	vector2_input->value = std::make_shared<Float3SocketValue>(1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f, 1.0f, -100000.0f, 100000.0f);
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

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto wavelength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Wavelength", "wavelength");
	wavelength_input->value = std::make_shared<FloatSocketValue>(500.0f, 380.0f, 780.0f);

	sockets.push_back(wavelength_input);

	type = CyclesNodeType::Wavelength;
}
