#include "node_colors.h"

#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "curve.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::MixRGBNode::MixRGBNode(FloatPos position)
{
	world_pos = position;

	title = "Mix RGB";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> type_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Type", "type");
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
	const std::shared_ptr<NodeSocket> clamp_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Boolean, "Clamp", "use_clamp");
	clamp_input->value = new BoolSocketValue(false);
	const std::shared_ptr<NodeSocket> fac_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> color1_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color1", "color1");
	color1_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> color2_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color2", "color2");
	color2_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(type_input);
	sockets.push_back(clamp_input);
	sockets.push_back(fac_input);
	sockets.push_back(color1_input);
	sockets.push_back(color2_input);

	type = CyclesNodeType::MixRGB;
}

CyclesShaderEditor::InvertNode::InvertNode(FloatPos position)
{
	world_pos = position;

	title = "Invert";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> fac_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(fac_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::Invert;
}

CyclesShaderEditor::LightFalloffNode::LightFalloffNode(FloatPos position)
{
	world_pos = position;

	title = "Light Falloff";

	const std::shared_ptr<NodeSocket> quadratic_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Quadratic", "quadratic");
	const std::shared_ptr<NodeSocket> linear_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Linear", "linear");
	const std::shared_ptr<NodeSocket> constant_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Constant", "constant");

	sockets.push_back(quadratic_output);
	sockets.push_back(linear_output);
	sockets.push_back(constant_output);

	const std::shared_ptr<NodeSocket> strength_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(100.0f, 0.0f, 10000.0f);
	const std::shared_ptr<NodeSocket> smooth_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Smooth", "smooth");
	smooth_input->value = new FloatSocketValue(0.0f, 0.0f, 1000.0f);

	sockets.push_back(strength_input);
	sockets.push_back(smooth_input);

	type = CyclesNodeType::LightFalloff;
}

CyclesShaderEditor::HSVNode::HSVNode(FloatPos position)
{
	world_pos = position;

	title = "HSV";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> hue_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Hue", "hue");
	hue_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> sat_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Saturation", "saturation");
	sat_input->value = new FloatSocketValue(1.0f, 0.0f, 2.0f);
	const std::shared_ptr<NodeSocket> val_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Value", "value");
	val_input->value = new FloatSocketValue(1.0f, 0.0f, 2.0f);
	const std::shared_ptr<NodeSocket> fac_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(hue_input);
	sockets.push_back(sat_input);
	sockets.push_back(val_input);
	sockets.push_back(fac_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::HSV;
}

CyclesShaderEditor::GammaNode::GammaNode(FloatPos position)
{
	world_pos = position;

	title = "Gamma";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> gamma_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Gamma", "gamma");
	gamma_input->value = new FloatSocketValue(1.0f, 0.001f, 10.0f);

	sockets.push_back(color_input);
	sockets.push_back(gamma_input);

	type = CyclesNodeType::Gamma;
}

CyclesShaderEditor::BrightnessContrastNode::BrightnessContrastNode(FloatPos position)
{
	world_pos = position;

	title = "Bright/Contrast";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> bright_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Bright", "bright");
	bright_input->value = new FloatSocketValue(0.0f, -100.0f, 100.0f);
	const std::shared_ptr<NodeSocket> cont_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Contrast", "contrast");
	cont_input->value = new FloatSocketValue(0.0f, -100.0f, 100.0f);

	sockets.push_back(color_input);
	sockets.push_back(bright_input);
	sockets.push_back(cont_input);

	type = CyclesNodeType::BrightnessContrast;
}

CyclesShaderEditor::RGBCurvesNode::RGBCurvesNode(FloatPos position)
{
	world_pos = position;

	title = "RGB Curves";

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> rgb_curve_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Curve, "RGB Curve", "rgb_curve");
	rgb_curve_input->value = new CurveSocketValue();
	const std::shared_ptr<NodeSocket> r_curve_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Curve, "Red Curve", "r_curve");
	r_curve_input->value = new CurveSocketValue();
	const std::shared_ptr<NodeSocket> g_curve_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Curve, "Green Curve", "g_curve");
	g_curve_input->value = new CurveSocketValue();
	const std::shared_ptr<NodeSocket> b_curve_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Curve, "Blue Curve", "b_curve");
	b_curve_input->value = new CurveSocketValue();
	const std::shared_ptr<NodeSocket> fac_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(1.0f, -1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(rgb_curve_input);
	sockets.push_back(r_curve_input);
	sockets.push_back(g_curve_input);
	sockets.push_back(b_curve_input);
	sockets.push_back(fac_input);
	sockets.push_back(color_input);

	type = CyclesNodeType::RGBCurves;
}

void CyclesShaderEditor::RGBCurvesNode::update_output_node(OutputNode& output)
{
	EditorNode::update_output_node(output);

	// Special case for RGB curves, we need to calculate the final output for the red, green, and blue channels
	// This is a combination of the channel-specific curves and the RGB curve which applies to all channels
	// Channel-specific curves are applied before the rgb curve

	const std::shared_ptr<NodeSocket> rgb_curve_socket = get_socket_by_internal_name(SocketIOType::Input, "rgb_curve").lock();
	const std::shared_ptr<NodeSocket> r_curve_socket = get_socket_by_internal_name(SocketIOType::Input, "r_curve").lock();
	const std::shared_ptr<NodeSocket> g_curve_socket = get_socket_by_internal_name(SocketIOType::Input, "g_curve").lock();
	const std::shared_ptr<NodeSocket> b_curve_socket = get_socket_by_internal_name(SocketIOType::Input, "b_curve").lock();

	CurveSocketValue* const rgb_curve_val = dynamic_cast<CurveSocketValue*>(rgb_curve_socket->value);
	CurveSocketValue* const r_curve_val = dynamic_cast<CurveSocketValue*>(r_curve_socket->value);
	CurveSocketValue* const g_curve_val = dynamic_cast<CurveSocketValue*>(g_curve_socket->value);
	CurveSocketValue* const b_curve_val = dynamic_cast<CurveSocketValue*>(b_curve_socket->value);

	CurveEvaluator rgb_curve(rgb_curve_val);
	CurveEvaluator r_curve(r_curve_val);
	CurveEvaluator g_curve(g_curve_val);
	CurveEvaluator b_curve(b_curve_val);

	OutputCurve out_r_curve;
	OutputCurve out_g_curve;
	OutputCurve out_b_curve;

	for (unsigned int i = 0; i < CURVE_TABLE_SIZE; i++) {
		const float x = static_cast<float>(i) / (CURVE_TABLE_SIZE - 1.0f);
		out_r_curve.samples.push_back(rgb_curve.eval(r_curve.eval(x)));
		out_g_curve.samples.push_back(rgb_curve.eval(g_curve.eval(x)));
		out_b_curve.samples.push_back(rgb_curve.eval(b_curve.eval(x)));
	}

	output.curve_values["final_r_curve"] = out_r_curve;
	output.curve_values["final_g_curve"] = out_g_curve;
	output.curve_values["final_b_curve"] = out_b_curve;
}
