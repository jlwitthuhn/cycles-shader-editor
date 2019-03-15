#include "node_shaders.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

cse::AmbientOcculsionNode::AmbientOcculsionNode(FloatPos position)
{
	world_pos = position;

	title = "Ambient Occlusion";

	const auto ao_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "AO", "AO");

	sockets.push_back(ao_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);

	sockets.push_back(color_input);

	type = CyclesNodeType::AmbientOcclusion;
}

cse::PrincipledBSDFNode::PrincipledBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Principled BSDF";

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
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");
	const auto cc_normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Clearcoat Normal", "clearcoat_normal");
	const auto tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::VECTOR, "Tangent", "tangent");

	sockets.push_back(dist_input);
	sockets.push_back(base_color_input);
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
	sockets.push_back(normal_input);
	sockets.push_back(cc_normal_input);
	sockets.push_back(tangent_input);

	type = CyclesNodeType::PrincipledBSDF;

	content_width += 34.0f;
}

cse::MixShaderNode::MixShaderNode(FloatPos position)
{
	world_pos = position;

	title = "Mix Shader";

	const auto shader_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Closure", "closure");

	sockets.push_back(shader_output);

	const auto fac_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Fac", "fac");
	fac_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure1", "closure1");
	const auto shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure2", "closure2");

	sockets.push_back(fac_input);
	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);

	type = CyclesNodeType::MixShader;
}

cse::AddShaderNode::AddShaderNode(FloatPos position)
{
	world_pos = position;

	title = "Add Shader";

	const auto shader_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Closure", "closure");

	sockets.push_back(shader_output);

	const auto shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure1", "closure1");
	const auto shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::CLOSURE, "Closure2", "closure2");

	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);

	type = CyclesNodeType::AddShader;
}

cse::DiffuseBSDFNode::DiffuseBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Diffuse BSDF";

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

	type = CyclesNodeType::DiffuseBSDF;
}

cse::GlossyBSDFNode::GlossyBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Glossy BSDF";

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

	type = CyclesNodeType::GlossyBSDF;
}

cse::TransparentBSDFNode::TransparentBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Transparent BSDF";

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);

	sockets.push_back(color_input);

	type = CyclesNodeType::TransparentBSDF;
}


cse::RefractionBSDFNode::RefractionBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Refraction BSDF";

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

	type = CyclesNodeType::RefractionBSDF;
}

cse::GlassBSDFNode::GlassBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Glass BSDF";

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

	type = CyclesNodeType::GlassBSDF;
}

cse::TranslucentBSDFNode::TranslucentBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Translucent BSDF";

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSDF", "bsdf");

	sockets.push_back(bsdf_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::TranslucentBSDF;
}

cse::AnisotropicBSDFNode::AnisotropicBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Anisotropic BSDF";

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

	type = CyclesNodeType::AnisotropicBSDF;
}

cse::VelvetBSDFNode::VelvetBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Velvet BSDF";

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

	type = CyclesNodeType::VelvetBSDF;
}

cse::ToonBSDFNode::ToonBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Toon BSDF";

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

	type = CyclesNodeType::ToonBSDF;
}

cse::SubsurfaceScatteringNode::SubsurfaceScatteringNode(FloatPos position)
{
	world_pos = position;

	title = "Subsurface Scattering";

	const auto bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "BSSRDF", "BSSRDF");

	sockets.push_back(bsdf_output);

	const auto comp_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Falloff", "falloff");
	const auto comp_value = std::make_shared<StringEnumSocketValue>();
	comp_value->enum_values.push_back(StringEnumPair("Christensen-Burley", "burley"));
	comp_value->enum_values.push_back(StringEnumPair("Gaussian", "gaussian"));
	comp_value->enum_values.push_back(StringEnumPair("Cubic", "cubic"));
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

	type = CyclesNodeType::SubsurfaceScattering;

	content_width += 20.0f;
}

cse::EmissionNode::EmissionNode(FloatPos position)
{
	world_pos = position;

	title = "Emission";

	const auto emission_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Emission", "emission");

	sockets.push_back(emission_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto strength_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Strength", "strength");
	strength_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(strength_input);

	type = CyclesNodeType::Emission;
}

cse::HairBSDFNode::HairBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Hair BSDF";

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

	type = CyclesNodeType::HairBSDF;
}

cse::HoldoutNode::HoldoutNode(FloatPos position)
{
	world_pos = position;

	title = "Holdout";

	const auto holdout_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Holdout", "holdout");

	sockets.push_back(holdout_output);

	type = CyclesNodeType::Holdout;
}

cse::VolumeAbsorptionNode::VolumeAbsorptionNode(FloatPos position)
{
	world_pos = position;

	title = "Volume Absorption";

	const auto vol_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::CLOSURE, "Volume", "volume");

	sockets.push_back(vol_output);

	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto density_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Density", "density");
	density_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);

	type = CyclesNodeType::VolAbsorption;
}

cse::VolumeScatterNode::VolumeScatterNode(FloatPos position)
{
	world_pos = position;

	title = "Volume Scatter";

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

	type = CyclesNodeType::VolScatter;
}
