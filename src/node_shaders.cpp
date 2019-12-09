#include "node_shaders.h"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

cse::PrincipledBSDFNode::PrincipledBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::PrincipledBSDF, "Principled BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto dist_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distribution", "distribution");
	const auto dist_value = std::make_shared<StringEnumSocketValue>();
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->set_from_internal_name("multiscatter_ggx");
	dist_input->value = dist_value;
	const auto base_color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Base Color", "base_color");
	base_color_input->value = std::make_shared<ColorSocketValue>(0.8f, 0.8f, 0.8f);

	const auto sss_method_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Subsurface Method", "subsurface_method");
	const auto sss_method_value = std::make_shared<StringEnumSocketValue>();
	sss_method_value->enum_values.push_back(StringEnumPair("Burley", "burley"));
	sss_method_value->enum_values.push_back(StringEnumPair("Random Walk", "random_walk"));
	sss_method_value->set_from_internal_name("burley");
	sss_method_input->value = sss_method_value;
	const auto ss_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Subsurface", "subsurface");
	ss_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto ss_radius_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Subsurface Radius", "subsurface_radius");
	ss_radius_input->value = std::make_shared<Float3SocketValue>(
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f);
	const auto ss_color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Subsurface Color", "subsurface_color");
	ss_color_input->value = std::make_shared<ColorSocketValue>(0.7f, 0.1f, 0.1f);

	const auto metallic_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Metallic", "metallic");
	metallic_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto spec_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Specular", "specular");
	spec_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto spec_tint_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Specular Tint", "specular_tint");
	spec_tint_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);

	const auto anisotropic_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Anisotropic", "anisotropic");
	anisotropic_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto anisotropic_rot_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Rotation", "anisotropic_rotation");
	anisotropic_rot_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	const auto sheen_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Sheen", "sheen");
	sheen_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto sheen_tint_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Sheen Tint", "sheen_tint");
	sheen_tint_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);

	const auto cc_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Clearcoat", "clearcoat");
	cc_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto cc_roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Clearcoat Roughness", "clearcoat_roughness");
	cc_roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "ior");
	ior_input->value = std::make_shared<FloatSocketValue>(1.45f, 0.0f, 100.0f);
	const auto transmission_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Transmission", "transmission");
	transmission_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);

	const auto emission_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Emission", "emission");
	emission_input->value = std::make_shared<ColorSocketValue>(0.0f, 0.0f, 0.0f);
	const auto alpha_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Alpha", "alpha");
	alpha_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);

	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");
	const auto cc_normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Clearcoat Normal", "clearcoat_normal");
	const auto tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Tangent", "tangent");

	sockets.push_back(dist_input);
	sockets.push_back(base_color_input);
	sockets.push_back(sss_method_input);
	sockets.push_back(ss_input);
	sockets.push_back(ss_radius_input);
	sockets.push_back(ss_color_input);
	sockets.push_back(metallic_input);
	sockets.push_back(spec_input);
	sockets.push_back(spec_tint_input);
	sockets.push_back(roughness_input);
	sockets.push_back(anisotropic_input);
	sockets.push_back(anisotropic_rot_input);
	sockets.push_back(sheen_input);
	sockets.push_back(sheen_tint_input);
	sockets.push_back(cc_input);
	sockets.push_back(cc_roughness_input);
	sockets.push_back(ior_input);
	sockets.push_back(transmission_input);
	sockets.push_back(emission_input);
	sockets.push_back(alpha_input);
	sockets.push_back(normal_input);
	sockets.push_back(cc_normal_input);
	sockets.push_back(tangent_input);

	content_width += 34.0f;
}

cse::PrincipledVolumeNode::PrincipledVolumeNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::PrincipledVolume, "Principled Volume")
{
	world_pos = position;

	const auto volume_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Volume", "volume");

	sockets.push_back(volume_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.5f, 0.5f, 0.5f);
	const auto density_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Density", "density");
	density_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 5000.0f);
	const auto anisotropy_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Anisotropy", "anisotropy");
	anisotropy_input->value = std::make_shared<FloatSocketValue>(0.0f, -1.0f, 1.0f);

	const auto absorption_color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Absorption Color", "absorption_color");
	absorption_color_input->value = std::make_shared<ColorSocketValue>(0.0f, 0.0f, 0.0f);
	const auto emission_strength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Emission Strength", "emission_strength");
	emission_strength_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 5000.0f);
	const auto emission_color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Emission Color", "emission_color");
	emission_color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto blackbody_intensity_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Blackbody Intensity", "blackbody_intensity");
	blackbody_intensity_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto blackbody_tint_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Blackbody Tint", "blackbody_tint");
	blackbody_tint_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto temperature_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Temperature", "temperature");
	temperature_input->value = std::make_shared<FloatSocketValue>(1500.0f, 0.0f, 5000.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);
	sockets.push_back(anisotropy_input);
	sockets.push_back(absorption_color_input);
	sockets.push_back(emission_strength_input);
	sockets.push_back(emission_color_input);
	sockets.push_back(blackbody_intensity_input);
	sockets.push_back(blackbody_tint_input);
	sockets.push_back(temperature_input);

	content_width += 28.0f;
}

cse::PrincipledHairNode::PrincipledHairNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::PrincipledHair, "Principled Hair")
{
	constexpr float PI = 3.14159f;

	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto coloring_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Coloring", "coloring");
	const auto coloring_value = std::make_shared<StringEnumSocketValue>();
	coloring_value->enum_values.push_back(StringEnumPair("Absorption coefficient", "absorption_coefficient"));
	coloring_value->enum_values.push_back(StringEnumPair("Melanin concentration", "melanin_concentration"));
	coloring_value->enum_values.push_back(StringEnumPair("Direct coloring", "direct_coloring"));
	coloring_value->set_from_internal_name("direct_coloring");
	coloring_input->value = coloring_value;

	// Absorption coefficient
	const auto absorption_coefficient_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Absorption Coefficient", "absorption_coefficient");
	absorption_coefficient_input->value = std::make_shared<Float3SocketValue>(
		0.245531f, 0.0f, 1000.0f,
		0.52f,     0.0f, 1000.0f,
		1.365f,    0.0f, 1000.0f);
	absorption_coefficient_input->selectable = true;

	// Melanin concentration
	const auto melanin_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Melanin", "melanin");
	melanin_input->value = std::make_shared<FloatSocketValue>(0.8f, 0.0f, 1.0f);
	const auto melanin_redness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Melanin Redness", "melanin_redness");
	melanin_redness_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);
	const auto tint_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Tint", "tint");
	tint_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);

	// Direct coloring
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(0.017513f, 0.005763f, 0.002059f);

	// General
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.3f, 0.0f, 1.0f);
	const auto radial_roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "radial_roughness");
	radial_roughness_input->value = std::make_shared<FloatSocketValue>(0.3f, 0.0f, 1.0f);
	const auto coat_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Coat", "coat");
	coat_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "ior");
	ior_input->value = std::make_shared<FloatSocketValue>(1.55f, 0.0f, 1000.0f);
	const auto offset_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Offset", "offset");
	offset_input->value = std::make_shared<FloatSocketValue>(2.0f * PI / 180.f, 0.0f, PI / 2.0f);
	const auto random_roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Random Roughness", "random_roughness");
	random_roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto random_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Random", "random");
	random_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1000.0f);

	sockets.push_back(coloring_input);
	sockets.push_back(absorption_coefficient_input);
	sockets.push_back(melanin_input);
	sockets.push_back(melanin_redness_input);
	sockets.push_back(tint_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(radial_roughness_input);
	sockets.push_back(coat_input);
	sockets.push_back(ior_input);
	sockets.push_back(offset_input);
	sockets.push_back(random_roughness_input);
	sockets.push_back(random_input);

	content_width += 62.0f;
}

cse::MixShaderNode::MixShaderNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::MixShader, "Mix Shader")
{
	world_pos = position;

	const auto shader_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Closure", "closure");

	sockets.push_back(shader_output);

	const auto fac_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Fac", "fac");
	fac_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure1", "closure1");
	const auto shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure2", "closure2");

	sockets.push_back(fac_input);
	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);
}

cse::AddShaderNode::AddShaderNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::AddShader, "Add Shader")
{
	world_pos = position;

	const auto shader_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Closure", "closure");

	sockets.push_back(shader_output);

	const auto shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure1", "closure1");
	const auto shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure2", "closure2");

	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);
}

cse::DiffuseBSDFNode::DiffuseBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::DiffuseBSDF, "Diffuse BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(normal_input);
}

cse::GlossyBSDFNode::GlossyBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::GlossyBSDF, "Glossy BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto dist_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distribution", "distribution");
	const auto dist_value = std::make_shared<StringEnumSocketValue>();
	dist_value->enum_values.push_back(StringEnumPair("Ashikhmin-Shirley", "ashikhmin_shirley"));
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(normal_input);
}

cse::TransparentBSDFNode::TransparentBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::TransparentBSDF, "Transparent BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);

	sockets.push_back(color_input);
}


cse::RefractionBSDFNode::RefractionBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::RefractionBSDF, "Refraction BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto dist_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distribution", "distribution");
	const auto dist_value = std::make_shared<StringEnumSocketValue>();
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "IOR");
	ior_input->value = std::make_shared<FloatSocketValue>(1.45f, 0.0f, 100.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(ior_input);
	sockets.push_back(normal_input);
}

cse::GlassBSDFNode::GlassBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::GlassBSDF, "Glass BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto dist_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distribution", "distribution");
	const auto dist_value = std::make_shared<StringEnumSocketValue>();
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "IOR");
	ior_input->value = std::make_shared<FloatSocketValue>(1.45f, 0.0f, 100.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(ior_input);
	sockets.push_back(normal_input);
}

cse::TranslucentBSDFNode::TranslucentBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::TranslucentBSDF, "Translucent BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "bsdf");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(normal_input);
}

cse::AnisotropicBSDFNode::AnisotropicBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::AnisotropicBSDF, "Anisotropic BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto dist_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Distribution", "distribution");
	const auto dist_value = std::make_shared<StringEnumSocketValue>();
	dist_value->enum_values.push_back(StringEnumPair("Ashikhmin-Shirley", "ashikhmin_shirley"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Roughness", "roughness");
	roughness_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto aniso_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Anisotropy", "anisotropy");
	aniso_input->value = std::make_shared<FloatSocketValue>(0.5f, -1.0f, 1.0f);
	const auto rotation_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Rotation", "rotation");
	rotation_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");
	const auto tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Tangent", "tangent");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(aniso_input);
	sockets.push_back(rotation_input);
	sockets.push_back(normal_input);
	sockets.push_back(tangent_input);
}

cse::VelvetBSDFNode::VelvetBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::VelvetBSDF, "Velvet BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto sigma_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Sigma", "sigma");
	sigma_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(sigma_input);
	sockets.push_back(normal_input);
}

cse::ToonBSDFNode::ToonBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::ToonBSDF, "Toon BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto comp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Component", "component");
	const auto comp_value = std::make_shared<StringEnumSocketValue>();
	comp_value->enum_values.push_back(StringEnumPair("Diffuse", "diffuse"));
	comp_value->enum_values.push_back(StringEnumPair("Glossy", "glossy"));
	comp_value->set_from_internal_name("diffuse");
	comp_input->value = comp_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Size", "size");
	size_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto smooth_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Smooth", "smooth");
	smooth_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(size_input);
	sockets.push_back(smooth_input);
	sockets.push_back(normal_input);
}

cse::SubsurfaceScatteringNode::SubsurfaceScatteringNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::SubsurfaceScattering, "Subsurface Scattering")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSSRDF", "BSSRDF");

	sockets.push_back(bsdf_output);

	const auto comp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Falloff", "falloff");
	const auto comp_value = std::make_shared<StringEnumSocketValue>();
	comp_value->enum_values.push_back(StringEnumPair("Christensen-Burley", "burley"));
	comp_value->enum_values.push_back(StringEnumPair("Gaussian", "gaussian"));
	comp_value->enum_values.push_back(StringEnumPair("Cubic", "cubic"));
	comp_value->enum_values.push_back(StringEnumPair("Random Walk", "random_walk"));
	comp_value->set_from_internal_name("burley");
	comp_input->value = comp_value;
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto scale_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Scale", "scale");
	scale_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1000.0f);
	const auto radius_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Radius", "radius");
	radius_input->value = std::make_shared<Float3SocketValue>(
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f);
	radius_input->selectable = true;
	const auto blur_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Texture Blur", "texture_blur");
	blur_input->value = std::make_shared<FloatSocketValue>(0.0f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(scale_input);
	sockets.push_back(radius_input);
	sockets.push_back(blur_input);
	sockets.push_back(normal_input);

	content_width += 20.0f;
}

cse::EmissionNode::EmissionNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::Emission, "Emission")
{
	world_pos = position;

	const auto emission_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Emission", "emission");

	sockets.push_back(emission_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto strength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Strength", "strength");
	strength_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(strength_input);
}

cse::HairBSDFNode::HairBSDFNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::HairBSDF, "Hair BSDF")
{
	world_pos = position;

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto comp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Component", "component");
	const auto comp_value = std::make_shared<StringEnumSocketValue>();
	comp_value->enum_values.push_back(StringEnumPair("Transmission", "transmission"));
	comp_value->enum_values.push_back(StringEnumPair("Reflection", "reflection"));
	comp_value->set_from_internal_name("reflection");
	comp_input->value = comp_value;

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto offset_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Offset", "offset");
	offset_input->value = std::make_shared<FloatSocketValue>(0.0f, -90.0f, 90.0f);
	const auto roughness_u_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "RoughnessU", "roughness_u");
	roughness_u_input->value = std::make_shared<FloatSocketValue>(0.1f, 0.0f, 1.0f);
	const auto roughness_v_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "RoughnessV", "roughness_v");
	roughness_v_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 1.0f);
	const auto tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Tangent", "tangent");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(offset_input);
	sockets.push_back(roughness_u_input);
	sockets.push_back(roughness_v_input);
	sockets.push_back(tangent_input);
}

cse::HoldoutNode::HoldoutNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::Holdout, "Holdout")
{
	world_pos = position;

	const auto holdout_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Holdout", "holdout");

	sockets.push_back(holdout_output);
}

cse::VolumeAbsorptionNode::VolumeAbsorptionNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::VolAbsorption, "Volume Absorption")
{
	world_pos = position;

	const auto vol_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Volume", "volume");

	sockets.push_back(vol_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto density_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Density", "density");
	density_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);
}

cse::VolumeScatterNode::VolumeScatterNode(FloatPos position) : EditableNode(NodeCategory::SHADER, CyclesNodeType::VolScatter, "Volume Scatter")
{
	world_pos = position;

	const auto vol_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Volume", "volume");

	sockets.push_back(vol_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto density_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Density", "density");
	density_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 100000.0f);
	const auto anisotropy_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Anisotropy", "anisotropy");
	anisotropy_input->value = std::make_shared<FloatSocketValue>(0.0f, -1.0f, 1.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);
	sockets.push_back(anisotropy_input);
}
