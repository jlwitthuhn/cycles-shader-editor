#include "node_converter.h"

#include <memory>
#include <vector>

#include "output.h"
#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::BlackbodyNode::BlackbodyNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::Blackbody, "Blackbody")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto temp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Temperature", "temperature");
	temp_input->value = std::make_shared<FloatSocketValue>(1500.0f, 800.0f, 20000.0f);

	sockets.push_back(temp_input);

	content_width += 14.0f;
}

cse::ColorRampNode::ColorRampNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::ColorRamp, "Color Ramp")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto alpha_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Alpha", "alpha");

	sockets.push_back(color_output);
	sockets.push_back(alpha_output);

	const auto ramp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR_RAMP, "Ramp", "ramp");
	ramp_input->value = std::make_shared<ColorRampSocketValue>();
	const auto fac_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Fac", "fac");
	fac_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	sockets.push_back(ramp_input);
	sockets.push_back(fac_input);
}

void cse::ColorRampNode::update_output_node(OutputNode& output)
{
	EditableNode::update_output_node(output);
}

cse::CombineHSVNode::CombineHSVNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::CombineHSV, "Combine HSV")
{
	world_pos = position;

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
}

cse::CombineRGBNode::CombineRGBNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::CombineRGB, "Combine RGB")
{
	world_pos = position;

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
}

cse::CombineXYZNode::CombineXYZNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::CombineXYZ, "Combine XYZ")
{
	world_pos = position;

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
}

cse::MathNode::MathNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::Math, "Math")
{
	world_pos = position;

	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(value_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Add (2)", "add"));
	type_value->enum_values.push_back(StringEnumPair("Subtract (2)", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Multiply (2)", "multiply"));
	type_value->enum_values.push_back(StringEnumPair("Divide (2)", "divide"));
	type_value->enum_values.push_back(StringEnumPair("Multiply Add (3)", "multiply_add"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Power (2)", "power"));
	type_value->enum_values.push_back(StringEnumPair("Logarithm (2)", "logarithm"));
	type_value->enum_values.push_back(StringEnumPair("Square Root (1)", "sqrt"));
	type_value->enum_values.push_back(StringEnumPair("Inverse Square Root (1)", "inversesqrt"));
	type_value->enum_values.push_back(StringEnumPair("Absolute (1)", "absolute"));
	type_value->enum_values.push_back(StringEnumPair("Exponent (1)", "exponent"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Minimum (2)", "minimum"));
	type_value->enum_values.push_back(StringEnumPair("Maximum (2)", "maximum"));
	type_value->enum_values.push_back(StringEnumPair("Less Than (2)", "less_than"));
	type_value->enum_values.push_back(StringEnumPair("Greater Than (2)", "greater_than"));
	type_value->enum_values.push_back(StringEnumPair("Sign (1)", "sign"));
	type_value->enum_values.push_back(StringEnumPair("Compare (3)", "compare"));
	type_value->enum_values.push_back(StringEnumPair("Smooth Minimum (3)", "smoothmin"));
	type_value->enum_values.push_back(StringEnumPair("Smooth Maximum (3)", "smoothmax"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Round (1)", "round"));
	type_value->enum_values.push_back(StringEnumPair("Floor (1)", "floor"));
	type_value->enum_values.push_back(StringEnumPair("Ceil (1)", "ceil"));
	type_value->enum_values.push_back(StringEnumPair("Truncate (1)", "trunc"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Fraction (1)", "fraction"));
	type_value->enum_values.push_back(StringEnumPair("Modulo (2)", "modulo"));
	type_value->enum_values.push_back(StringEnumPair("Snap (2)", "snap"));
	type_value->enum_values.push_back(StringEnumPair("Wrap (3)", "wrap"));
	type_value->enum_values.push_back(StringEnumPair("Pingpong (2)", "pingpong"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Sine (1)", "sine"));
	type_value->enum_values.push_back(StringEnumPair("Cosine (1)", "cosine"));
	type_value->enum_values.push_back(StringEnumPair("Tangent (1)", "tangent"));
	type_value->enum_values.push_back(StringEnumPair("Arcsine (1)", "arcsine"));
	type_value->enum_values.push_back(StringEnumPair("Arccosine (1)", "arccosine"));
	type_value->enum_values.push_back(StringEnumPair("Arctangent (1)", "arctangent"));
	type_value->enum_values.push_back(StringEnumPair("Arctan2 (2)", "arctan2"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Hyperbolic Sine (1)", "sinh"));
	type_value->enum_values.push_back(StringEnumPair("Hyperbolic Cosine (1)", "cosh"));
	type_value->enum_values.push_back(StringEnumPair("Hyperbolic Tangent (1)", "tanh"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("To Radians (1)", "radians"));
	type_value->enum_values.push_back(StringEnumPair("To Degrees (1)", "degrees"));
	type_value->set_from_internal_name("add");
	type_input->value = type_value;
	const auto clamp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Clamp", "use_clamp");
	clamp_input->value = std::make_shared<BoolSocketValue>(false);
	const auto value1_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value1", "value1");
	value1_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);
	const auto value2_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value2", "value2");
	value2_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);
	const auto value3_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value3", "value3");
	value3_input->value = std::make_shared<FloatSocketValue>(0.0f, -100000.0f, 100000.0f);

	sockets.push_back(type_input);
	sockets.push_back(clamp_input);
	sockets.push_back(value1_input);
	sockets.push_back(value2_input);
	sockets.push_back(value3_input);
}

cse::RGBToBWNode::RGBToBWNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::RGBtoBW, "RGB to BW")
{
	world_pos = position;

	const auto val_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Val", "val");

	sockets.push_back(val_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);
}

cse::SeparateHSVNode::SeparateHSVNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::SeparateHSV, "Separate HSV")
{
	world_pos = position;

	const auto h_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "H", "h");
	const auto s_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "S", "s");
	const auto v_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "V", "v");

	sockets.push_back(h_output);
	sockets.push_back(s_output);
	sockets.push_back(v_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(color_input);
}

cse::SeparateRGBNode::SeparateRGBNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::SeparateRGB, "Separate RGB")
{
	world_pos = position;

	const auto r_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "R", "r");
	const auto g_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "G", "g");
	const auto b_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "B", "b");

	sockets.push_back(r_output);
	sockets.push_back(g_output);
	sockets.push_back(b_output);

	const auto image_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Image", "image");
	image_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);

	sockets.push_back(image_input);
}

cse::SeparateXYZNode::SeparateXYZNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::SeparateXYZ, "Separate XYZ")
{
	world_pos = position;

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
}

cse::VectorMathNode::VectorMathNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::VectorMath, "Vector Math")
{
	world_pos = position;

	const auto vector_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Vector", "vector");
	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(vector_output);
	sockets.push_back(value_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Add", "add"));
	type_value->enum_values.push_back(StringEnumPair("Subtract", "subtract"));
	type_value->enum_values.push_back(StringEnumPair("Multiply", "multiply"));
	type_value->enum_values.push_back(StringEnumPair("Divide", "divide"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Cross Product", "cross_product"));
	type_value->enum_values.push_back(StringEnumPair("Project", "project"));
	type_value->enum_values.push_back(StringEnumPair("Reflect", "reflect"));
	type_value->enum_values.push_back(StringEnumPair("Dot Product", "dot_product"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Distance", "distance"));
	type_value->enum_values.push_back(StringEnumPair("Length", "length"));
	type_value->enum_values.push_back(StringEnumPair("Scale", "scale"));
	type_value->enum_values.push_back(StringEnumPair("Normalize", "normalize"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Absolute", "absolute"));
	type_value->enum_values.push_back(StringEnumPair("Minimum", "minimum"));
	type_value->enum_values.push_back(StringEnumPair("Maximum", "maximum"));
	type_value->enum_values.push_back(StringEnumPair("Floor", "floor"));
	type_value->enum_values.push_back(StringEnumPair("Ceil", "ceil"));
	type_value->enum_values.push_back(StringEnumPair("Fraction", "fraction"));
	type_value->enum_values.push_back(StringEnumPair("Modulo", "modulo"));
	type_value->enum_values.push_back(StringEnumPair("Snap", "snap"));
	type_value->enum_values.push_back(StringEnumPair::make_spacer());
	type_value->enum_values.push_back(StringEnumPair("Sine", "sine"));
	type_value->enum_values.push_back(StringEnumPair("Cosine", "cosine"));
	type_value->enum_values.push_back(StringEnumPair("Tangent", "tangent"));

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
}

cse::WavelengthNode::WavelengthNode(const Float2 position) : EditableNode(NodeCategory::CONVERTER, CyclesNodeType::Wavelength, "Wavelength")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto wavelength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Wavelength", "wavelength");
	wavelength_input->value = std::make_shared<FloatSocketValue>(500.0f, 380.0f, 780.0f);

	sockets.push_back(wavelength_input);
}
