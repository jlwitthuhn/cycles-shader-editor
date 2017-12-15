#include "node_textures.h"

CyclesShaderEditor::BrickTextureNode::BrickTextureNode(Point2 position)
{
	world_pos = position;

	title = "Brick Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* offset_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Offset", "offset");
	offset_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	offset_input->draw_socket = false;
	NodeSocket* offset_freq_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Offset Freq", "offset_frequency");
	offset_freq_input->value = new IntSocketValue(2, 1, 99);
	offset_freq_input->draw_socket = false;
	NodeSocket* squash_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Squash", "squash");
	squash_input->value = new FloatSocketValue(1.0f, 0.0f, 99.0f);
	squash_input->draw_socket = false;
	NodeSocket* squash_freq_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Squash Freq", "squash_frequency");
	squash_freq_input->value = new IntSocketValue(2, 1, 99);
	squash_freq_input->draw_socket = false;

	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* color1_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color1", "color1");
	color1_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	NodeSocket* color2_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color2", "color2");
	color2_input->value = new ColorSocketValue(0.5f, 0.5f, 0.5f);
	NodeSocket* mortar_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Mortar", "mortar");
	mortar_input->value = new ColorSocketValue(0.0f, 0.0f, 0.0f);
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(5.0f, -1000.0f, 1000.0f);
	NodeSocket* mortar_size_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Mortar Size", "mortar_size");
	mortar_size_input->value = new FloatSocketValue(0.02f, 0.0f, 0.25f);
	NodeSocket* mortar_smooth_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Mortar Smooth", "mortar_smooth");
	mortar_smooth_input->value = new FloatSocketValue(0.1f, 0.0f, 1.0f);
	NodeSocket* bias_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Bias", "bias");
	bias_input->value = new FloatSocketValue(0.0f, -1.0f, 1.0f);
	NodeSocket* brick_width_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Brick Width", "brick_width");
	brick_width_input->value = new FloatSocketValue(0.5f, 0.01f, 100.0f);
	NodeSocket* row_height_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Row Height", "row_height");
	row_height_input->value = new FloatSocketValue(0.25f, 0.01f, 100.0f);

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

CyclesShaderEditor::NoiseTextureNode::NoiseTextureNode(Point2 position)
{
	world_pos = position;

	title = "Noise Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);
	NodeSocket* detail_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Detail", "detail");
	detail_input->value = new FloatSocketValue(2.0f, 0.0f, 16.0f);
	NodeSocket* distortion_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Distortion", "distortion");
	distortion_input->value = new FloatSocketValue(0.0f, -1000.0f, 1000.0f);

	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(detail_input);
	sockets.push_back(distortion_input);

	type = CyclesNodeType::NoiseTex;
}

CyclesShaderEditor::WaveTextureNode::WaveTextureNode(Point2 position)
{
	world_pos = position;

	title = "Wave Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Bands", "bands"));
	type_value->enum_values.push_back(StringEnumPair("Rings", "rings"));
	type_value->set_from_internal_name("bands");
	type_input->value = type_value;
	NodeSocket* profile_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Profile", "profile");
	StringEnumSocketValue* profile_value = new StringEnumSocketValue();
	profile_value->enum_values.push_back(StringEnumPair("Saw", "saw"));
	profile_value->enum_values.push_back(StringEnumPair("Sine", "sine"));
	profile_value->set_from_internal_name("sine");
	profile_input->value = profile_value;
	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);
	NodeSocket* distortion_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Distortion", "distortion");
	distortion_input->value = new FloatSocketValue(0.0f, -1000.0f, 1000.0f);
	NodeSocket* detail_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Detail", "detail");
	detail_input->value = new FloatSocketValue(2.0f, 0.0f, 16.0f);
	NodeSocket* detail_scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Detail Scale", "detail_scale");
	detail_scale_input->value = new FloatSocketValue(1.0f, -1000.0f, 1000.0f);

	sockets.push_back(type_input);
	sockets.push_back(profile_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(distortion_input);
	sockets.push_back(detail_input);
	sockets.push_back(detail_scale_input);

	type = CyclesNodeType::WaveTex;
}

CyclesShaderEditor::VoronoiTextureNode::VoronoiTextureNode(Point2 position)
{
	world_pos = position;

	title = "Voronoi Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* coloring_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Coloring", "coloring");
	StringEnumSocketValue* coloring_value = new StringEnumSocketValue();
	coloring_value->enum_values.push_back(StringEnumPair("Cells", "cells"));
	coloring_value->enum_values.push_back(StringEnumPair("Intensity", "intensity"));
	coloring_value->set_from_internal_name("intensity");
	coloring_input->value = coloring_value;
	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);

	sockets.push_back(coloring_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);

	type = CyclesNodeType::VoronoiTex;
}

CyclesShaderEditor::MusgraveTextureNode::MusgraveTextureNode(Point2 position)
{
	world_pos = position;

	title = "Musgrave Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("fBM", "fBM"));
	type_value->enum_values.push_back(StringEnumPair("Hetero Terrain", "hetero_terrain"));
	type_value->enum_values.push_back(StringEnumPair("Hybrid Multifractal", "hybrid_multifractal"));
	type_value->enum_values.push_back(StringEnumPair("Multifractal", "multifractal"));
	type_value->enum_values.push_back(StringEnumPair("Ridged Multifractal", "ridged_multifractal"));
	type_value->set_from_internal_name("fBM");
	type_input->value = type_value;
	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);
	NodeSocket* detail_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Detail", "detail");
	detail_input->value = new FloatSocketValue(2.0f, 0.0f, 16.0f);
	NodeSocket* dimension_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Dimension", "dimension");
	dimension_input->value = new FloatSocketValue(2.0f, 0.0f, 1000.0f);
	NodeSocket* lacunarity_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Lacunarity", "lacunarity");
	lacunarity_input->value = new FloatSocketValue(1.0f, 0.0f, 1000.0f);
	NodeSocket* offset_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Offset", "offset");
	offset_input->value = new FloatSocketValue(0.0f, -1000.0f, 1000.0f);
	NodeSocket* gain_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Gain", "gain");
	gain_input->value = new FloatSocketValue(1.0f, 0.0f, 1000.0f);

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

CyclesShaderEditor::GradientTextureNode::GradientTextureNode(Point2 position)
{
	world_pos = position;

	title = "Gradient Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* type_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Type", "type");
	StringEnumSocketValue* type_value = new StringEnumSocketValue();
	type_value->enum_values.push_back(StringEnumPair("Linear", "linear"));
	type_value->enum_values.push_back(StringEnumPair("Quadratic", "quadratic"));
	type_value->enum_values.push_back(StringEnumPair("Easing", "easing"));
	type_value->enum_values.push_back(StringEnumPair("Diagonal", "diagonal"));
	type_value->enum_values.push_back(StringEnumPair("Radial", "radial"));
	type_value->enum_values.push_back(StringEnumPair("Quadratic Sphere", "quadratic_sphere"));
	type_value->enum_values.push_back(StringEnumPair("Spherical", "spherical"));
	type_value->set_from_internal_name("linear");
	type_input->value = type_value;
	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");

	sockets.push_back(type_input);
	sockets.push_back(vector_input);

	type = CyclesNodeType::GradientTex;
}

CyclesShaderEditor::MagicTextureNode::MagicTextureNode(Point2 position)
{
	world_pos = position;

	title = "Magic Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* depth_input = new NodeSocket(this, SocketInOut::Input, SocketType::Int, "Depth", "depth");
	depth_input->value = new IntSocketValue(2, 0, 10);
	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);
	NodeSocket* distortion_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Distortion", "distortion");
	distortion_input->value = new FloatSocketValue(1.0f, -1000.0f, 1000.0f);

	sockets.push_back(depth_input);
	sockets.push_back(vector_input);
	sockets.push_back(scale_input);
	sockets.push_back(distortion_input);

	type = CyclesNodeType::MagicTex;
}

CyclesShaderEditor::CheckerTextureNode::CheckerTextureNode(Point2 position)
{
	world_pos = position;

	title = "Checker Texture";

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");
	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(color_output);
	sockets.push_back(fac_output);

	NodeSocket* vector_input = new NodeSocket(this, SocketInOut::Input, SocketType::Vector, "Vector", "vector");
	NodeSocket* color1_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color1", "color1");
	color1_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	NodeSocket* color2_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Color2", "color2");
	color2_input->value = new ColorSocketValue(0.0f, 0.0f, 0.0f);
	NodeSocket* scale_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(0.5f, -1000.0f, 1000.0f);

	sockets.push_back(vector_input);
	sockets.push_back(color1_input);
	sockets.push_back(color2_input);
	sockets.push_back(scale_input);

	type = CyclesNodeType::CheckerTex;
}