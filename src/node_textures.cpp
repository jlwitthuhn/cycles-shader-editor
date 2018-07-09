#include "node_textures.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::BrickTextureNode::BrickTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Brick Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto offset_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Offset", "offset");
	offset_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	offset_input->draw_socket = false;
	const auto offset_freq_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Offset Freq", "offset_frequency");
	offset_freq_input->value = std::make_shared<IntSocketValue>(2, 1, 99);
	offset_freq_input->draw_socket = false;
	const auto squash_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Squash", "squash");
	squash_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 99.0f);
	squash_input->draw_socket = false;
	const auto squash_freq_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Squash Freq", "squash_frequency");
	squash_freq_input->value = std::make_shared<IntSocketValue>(2, 1, 99);
	squash_freq_input->draw_socket = false;

	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto color1_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color1", "color1");
	color1_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto color2_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color2", "color2");
	color2_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);
	const auto mortar_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Mortar", "mortar");
	mortar_input->value = std::make_shared<ColorSocketValue>(0.0f, 0.0f, 0.0f);
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(5.0f, -1000.0f, 1000.0f);
	const auto mortar_size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Mortar Size", "mortar_size");
	mortar_size_input->value = std::make_shared<FloatSocketValue>(0.02f, 0.0f, 0.25f);
	const auto mortar_smooth_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Mortar Smooth", "mortar_smooth");
	mortar_smooth_input->value = std::make_shared<FloatSocketValue>(0.1f, 0.0f, 1.0f);
	const auto bias_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Bias", "bias");
	bias_input->value = std::make_shared<FloatSocketValue>(0.0f, -1.0f, 1.0f);
	const auto brick_width_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Brick Width", "brick_width");
	brick_width_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.01f, 100.0f);
	const auto row_height_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Row Height", "row_height");
	row_height_input->value = std::make_shared<FloatSocketValue>(0.25f, 0.01f, 100.0f);

	sockets.push_back(offset_input);
	sockets.push_back(offset_freq_input);
	sockets.push_back(squash_input);
	sockets.push_back(squash_freq_input);

	sockets.push_back(vector_input);
	sockets.push_back(color1_input);
	sockets.push_back(color2_input);
	sockets.push_back(mortar_input);
	sockets.push_back(scale_input);
	sockets.push_back(mortar_size_input);
	sockets.push_back(mortar_smooth_input);
	sockets.push_back(bias_input);
	sockets.push_back(brick_width_input);
	sockets.push_back(row_height_input);

	type = CyclesNodeType::BrickTex;
}

CyclesShaderEditor::NoiseTextureNode::NoiseTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Noise Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);
	const auto detail_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Detail", "detail");
	detail_input->value = std::make_shared<FloatSocketValue>(2.0f, 0.0f, 16.0f);
	const auto distortion_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Distortion", "distortion");
	distortion_input->value = std::make_shared<FloatSocketValue>(0.0f, -1000.0f, 1000.0f);

	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(detail_input);
	sockets.push_back(distortion_input);

	type = CyclesNodeType::NoiseTex;
}

CyclesShaderEditor::WaveTextureNode::WaveTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Wave Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Bands", "bands"));
	type_value->enum_values.push_back(StringEnumPair("Rings", "rings"));
	type_value->set_from_internal_name("bands");
	type_input->value = type_value;
	const auto profile_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Profile", "profile");
	const auto profile_value = std::make_shared<StringEnumSocketValue>();
	profile_value->enum_values.push_back(StringEnumPair("Saw", "saw"));
	profile_value->enum_values.push_back(StringEnumPair("Sine", "sine"));
	profile_value->set_from_internal_name("sine");
	profile_input->value = profile_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);
	const auto distortion_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Distortion", "distortion");
	distortion_input->value = std::make_shared<FloatSocketValue>(0.0f, -1000.0f, 1000.0f);
	const auto detail_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Detail", "detail");
	detail_input->value = std::make_shared<FloatSocketValue>(2.0f, 0.0f, 16.0f);
	const auto detail_scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Detail Scale", "detail_scale");
	detail_scale_input->value = std::make_shared<FloatSocketValue>(1.0f, -1000.0f, 1000.0f);

	sockets.push_back(type_input);
	sockets.push_back(profile_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(distortion_input);
	sockets.push_back(detail_input);
	sockets.push_back(detail_scale_input);

	type = CyclesNodeType::WaveTex;
}

CyclesShaderEditor::VoronoiTextureNode::VoronoiTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Voronoi Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto coloring_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Coloring", "coloring");
	const auto coloring_value = std::make_shared<StringEnumSocketValue>();
	coloring_value->enum_values.push_back(StringEnumPair("Cells", "cells"));
	coloring_value->enum_values.push_back(StringEnumPair("Intensity", "intensity"));
	coloring_value->set_from_internal_name("intensity");
	coloring_input->value = coloring_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);

	sockets.push_back(coloring_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);

	type = CyclesNodeType::VoronoiTex;
}

CyclesShaderEditor::MusgraveTextureNode::MusgraveTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Musgrave Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("fBM", "fBM"));
	type_value->enum_values.push_back(StringEnumPair("Hetero Terrain", "hetero_terrain"));
	type_value->enum_values.push_back(StringEnumPair("Hybrid Multifractal", "hybrid_multifractal"));
	type_value->enum_values.push_back(StringEnumPair("Multifractal", "multifractal"));
	type_value->enum_values.push_back(StringEnumPair("Ridged Multifractal", "ridged_multifractal"));
	type_value->set_from_internal_name("fBM");
	type_input->value = type_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);
	const auto detail_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Detail", "detail");
	detail_input->value = std::make_shared<FloatSocketValue>(2.0f, 0.0f, 16.0f);
	const auto dimension_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Dimension", "dimension");
	dimension_input->value = std::make_shared<FloatSocketValue>(2.0f, 0.0f, 1000.0f);
	const auto lacunarity_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Lacunarity", "lacunarity");
	lacunarity_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1000.0f);
	const auto offset_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Offset", "offset");
	offset_input->value = std::make_shared<FloatSocketValue>(0.0f, -1000.0f, 1000.0f);
	const auto gain_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Gain", "gain");
	gain_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1000.0f);

	sockets.push_back(type_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(detail_input);
	sockets.push_back(dimension_input);
	sockets.push_back(lacunarity_input);
	sockets.push_back(offset_input);
	sockets.push_back(gain_input);

	type = CyclesNodeType::MusgraveTex;
}

CyclesShaderEditor::GradientTextureNode::GradientTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Gradient Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto type_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Type", "type");
	const auto type_value = std::make_shared<StringEnumSocketValue>();
	type_value->enum_values.push_back(StringEnumPair("Linear", "linear"));
	type_value->enum_values.push_back(StringEnumPair("Quadratic", "quadratic"));
	type_value->enum_values.push_back(StringEnumPair("Easing", "easing"));
	type_value->enum_values.push_back(StringEnumPair("Diagonal", "diagonal"));
	type_value->enum_values.push_back(StringEnumPair("Radial", "radial"));
	type_value->enum_values.push_back(StringEnumPair("Quadratic Sphere", "quadratic_sphere"));
	type_value->enum_values.push_back(StringEnumPair("Spherical", "spherical"));
	type_value->set_from_internal_name("linear");
	type_input->value = type_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");

	sockets.push_back(type_input);
	sockets.push_back(vector_input);

	type = CyclesNodeType::GradientTex;
}

CyclesShaderEditor::MagicTextureNode::MagicTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Magic Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto depth_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Depth", "depth");
	depth_input->value = std::make_shared<IntSocketValue>(2, 0, 10);
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);
	const auto distortion_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Distortion", "distortion");
	distortion_input->value = std::make_shared<FloatSocketValue>(1.0f, -1000.0f, 1000.0f);

	sockets.push_back(depth_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(distortion_input);

	type = CyclesNodeType::MagicTex;
}

CyclesShaderEditor::CheckerTextureNode::CheckerTextureNode(FloatPos position)
{
	world_pos = position;

	title = "Checker Texture";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto color1_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color1", "color1");
	color1_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto color2_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color2", "color2");
	color2_input->value = std::make_shared<ColorSocketValue>(0.0f, 0.0f, 0.0f);
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(0.5f, -1000.0f, 1000.0f);

	sockets.push_back(vector_input);
	sockets.push_back(color1_input);
	sockets.push_back(color2_input);
	sockets.push_back(scale_input);

	type = CyclesNodeType::CheckerTex;
}
