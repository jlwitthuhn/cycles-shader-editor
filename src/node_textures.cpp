#include "node_textures.h"

#include <memory>
#include <vector>

#include "output.h"
#include "sockets.h"
#include "util_enum.h"
#include "util_vector.h"

cse::BrickTextureNode::BrickTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::BrickTex, "Brick Texture")
{
	world_pos = position;

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
}

cse::NoiseTextureNode::NoiseTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::NoiseTex, "Noise Texture")
{
	world_pos = position;

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
}

cse::WaveTextureNode::WaveTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::WaveTex, "Wave Texture")
{
	world_pos = position;

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
}

cse::VoronoiTextureNode::VoronoiTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::VoronoiTex, "Voronoi Texture")
{
	world_pos = position;

	const auto distance_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Distance", "distance");
	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto position_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Position", "position");
	const auto w_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "W", "w");
	const auto radius_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Radius", "radius");

	sockets.push_back(distance_output);
	sockets.push_back(color_output);
	sockets.push_back(position_output);
	sockets.push_back(w_output);
	sockets.push_back(radius_output);

	const auto dimensions_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Dimensions", "dimensions");
	dimensions_input->value = std::make_shared<IntSocketValue>(3, 1, 4);
	const auto metric_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distance Metric", "metric");
	const auto metric_value = std::make_shared<StringEnumSocketValue>();
	metric_value->enum_values.push_back(StringEnumPair("Euclidian", "euclidian"));
	metric_value->enum_values.push_back(StringEnumPair("Manhattan", "manhattan"));
	metric_value->enum_values.push_back(StringEnumPair("Chebychev", "chebychev"));
	metric_value->enum_values.push_back(StringEnumPair("Minkowski", "minkowski"));
	metric_value->set_from_internal_name("euclidian");
	metric_input->value = metric_value;
	const auto feature_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Feature", "feature");
	const auto feature_value = std::make_shared<StringEnumSocketValue>();
	feature_value->enum_values.push_back(StringEnumPair("F1", "f1"));
	feature_value->enum_values.push_back(StringEnumPair("F2", "f2"));
	feature_value->enum_values.push_back(StringEnumPair("Smooth F1", "smooth_f1"));
	feature_value->enum_values.push_back(StringEnumPair("Distance to Edge", "distance_to_edge"));
	feature_value->enum_values.push_back(StringEnumPair("N-sphere Radius", "n_sphere_radius"));
	feature_value->set_from_internal_name("f1");
	feature_input->value = feature_value;
	const auto vector_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Vector", "vector");
	const auto w_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "W", "w");
	w_input->value = std::make_shared<FloatSocketValue>(0.0f, -1000.0f, 1000.0f);
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(5.0f, -1000.0f, 1000.0f);
	const auto smoothness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Smoothness", "smoothness");
	smoothness_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);
	const auto randomness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Randomness", "randomness");
	randomness_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);

	sockets.push_back(dimensions_input);
	sockets.push_back(metric_input);
	sockets.push_back(feature_input);
	sockets.push_back(vector_input);
	sockets.push_back(w_input);
	sockets.push_back(scale_input);
	sockets.push_back(smoothness_input);
	sockets.push_back(randomness_input);

	content_width += 12.0f;
}

cse::MusgraveTextureNode::MusgraveTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::MusgraveTex, "Musgrave Texture")
{
	world_pos = position;

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
}

cse::GradientTextureNode::GradientTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::GradientTex, "Gradient Texture")
{
	world_pos = position;

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
}

cse::MagicTextureNode::MagicTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::MagicTex, "Magic Texture")
{
	world_pos = position;

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
}

cse::CheckerTextureNode::CheckerTextureNode(const Float2 position) : EditableNode(NodeCategory::TEXTURE, CyclesNodeType::CheckerTex, "Checker Texture")
{
	world_pos = position;

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
}
