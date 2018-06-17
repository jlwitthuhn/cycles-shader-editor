#include "node_shaders.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::AmbientOcculsionNode::AmbientOcculsionNode(FloatPos position)
{
	world_pos = position;

	title = "Ambient Occlusion";

	const std::shared_ptr<NodeSocket> ao_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "AO", "AO");

	sockets.push_back(ao_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(color_input);

	type = CyclesNodeType::AmbientOcclusion;
}

CyclesShaderEditor::PrincipledBSDFNode::PrincipledBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Principled BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> dist_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Distribution", "distribution");
	StringEnumSocketValue* dist_value = new StringEnumSocketValue();
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->set_from_internal_name("multiscatter_ggx");
	dist_input->value = dist_value;
	const std::shared_ptr<NodeSocket> base_color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Base Color", "base_color");
	base_color_input->value = new ColorSocketValue(0.8f, 0.8f, 0.8f);
	const std::shared_ptr<NodeSocket> ss_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Subsurface", "subsurface");
	ss_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> ss_radius_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Subsurface Radius", "subsurface_radius");
	ss_radius_input->value = new Float3SocketValue(
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f);
	const std::shared_ptr<NodeSocket> ss_color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Subsurface Color", "subsurface_color");
	ss_color_input->value = new ColorSocketValue(0.7f, 0.1f, 0.1f);
	const std::shared_ptr<NodeSocket> metallic_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Metallic", "metallic");
	metallic_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> spec_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Specular", "specular");
	spec_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> spec_tint_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Specular Tint", "specular_tint");
	spec_tint_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> anisotropic_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Anisotropic", "anisotropic");
	anisotropic_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> anisotropic_rot_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Rotation", "anisotropic_rotation");
	anisotropic_rot_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> sheen_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Sheen", "sheen");
	sheen_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> sheen_tint_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Sheen Tint", "sheen_tint");
	sheen_tint_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> cc_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Clearcoat", "clearcoat");
	cc_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> cc_roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Clearcoat Roughness", "clearcoat_roughness");
	cc_roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> ior_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "IOR", "ior");
	ior_input->value = new FloatSocketValue(1.45f, 0.0f, 100.0f);
	const std::shared_ptr<NodeSocket> transmission_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Transmission", "transmission");
	transmission_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");
	const std::shared_ptr<NodeSocket> cc_normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Clearcoat Normal", "clearcoat_normal");
	const std::shared_ptr<NodeSocket> tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Tangent", "tangent");

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

CyclesShaderEditor::MixShaderNode::MixShaderNode(FloatPos position)
{
	world_pos = position;

	title = "Mix Shader";

	const std::shared_ptr<NodeSocket> shader_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Closure", "closure");

	sockets.push_back(shader_output);

	const std::shared_ptr<NodeSocket> fac_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Fac", "fac");
	fac_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Closure, "Closure1", "closure1");
	const std::shared_ptr<NodeSocket> shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Closure, "Closure2", "closure2");

	sockets.push_back(fac_input);
	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);

	type = CyclesNodeType::MixShader;
}

CyclesShaderEditor::AddShaderNode::AddShaderNode(FloatPos position)
{
	world_pos = position;

	title = "Add Shader";

	const std::shared_ptr<NodeSocket> shader_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Closure", "closure");

	sockets.push_back(shader_output);

	const std::shared_ptr<NodeSocket> shader_a_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Closure, "Closure1", "closure1");
	const std::shared_ptr<NodeSocket> shader_b_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Closure, "Closure2", "closure2");

	sockets.push_back(shader_a_input);
	sockets.push_back(shader_b_input);

	type = CyclesNodeType::AddShader;
}

CyclesShaderEditor::DiffuseBSDFNode::DiffuseBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Diffuse BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::DiffuseBSDF;
}

CyclesShaderEditor::GlossyBSDFNode::GlossyBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Glossy BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> dist_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Distribution", "distribution");
	StringEnumSocketValue* dist_value = new StringEnumSocketValue();
	dist_value->enum_values.push_back(StringEnumPair("Ashikhmin-Shirley", "ashikhmin_shirley"));
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::GlossyBSDF;
}

CyclesShaderEditor::TransparentBSDFNode::TransparentBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Transparent BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);

	sockets.push_back(color_input);

	type = CyclesNodeType::TransparentBSDF;
}


CyclesShaderEditor::RefractionBSDFNode::RefractionBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Refraction BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> dist_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Distribution", "distribution");
	StringEnumSocketValue* dist_value = new StringEnumSocketValue();
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> ior_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "IOR", "IOR");
	ior_input->value = new FloatSocketValue(1.45f, 0.0f, 100.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(ior_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::RefractionBSDF;
}

CyclesShaderEditor::GlassBSDFNode::GlassBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Glass BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> dist_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Distribution", "distribution");
	StringEnumSocketValue* dist_value = new StringEnumSocketValue();
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Sharp", "sharp"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> ior_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "IOR", "IOR");
	ior_input->value = new FloatSocketValue(1.45f, 0.0f, 100.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(ior_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::GlassBSDF;
}

CyclesShaderEditor::TranslucentBSDFNode::TranslucentBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Translucent BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "bsdf");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::TranslucentBSDF;
}

CyclesShaderEditor::AnisotropicBSDFNode::AnisotropicBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Anisotropic BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> dist_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Distribution", "distribution");
	StringEnumSocketValue* dist_value = new StringEnumSocketValue();
	dist_value->enum_values.push_back(StringEnumPair("Ashikhmin-Shirley", "ashikhmin_shirley"));
	dist_value->enum_values.push_back(StringEnumPair("Beckmann", "beckmann"));
	dist_value->enum_values.push_back(StringEnumPair("Multiscatter GGX", "multiscatter_ggx"));
	dist_value->enum_values.push_back(StringEnumPair("GGX", "ggx"));
	dist_value->set_from_internal_name("ggx");
	dist_input->value = dist_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Roughness", "roughness");
	roughness_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> aniso_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Anisotropy", "anisotropy");
	aniso_input->value = new FloatSocketValue(0.5f, -1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> rotation_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Rotation", "rotation");
	rotation_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");
	const std::shared_ptr<NodeSocket> tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Tangent", "tangent");

	sockets.push_back(dist_input);
	sockets.push_back(color_input);
	sockets.push_back(roughness_input);
	sockets.push_back(aniso_input);
	sockets.push_back(rotation_input);
	sockets.push_back(normal_input);
	sockets.push_back(tangent_input);

	type = CyclesNodeType::AnisotropicBSDF;
}

CyclesShaderEditor::VelvetBSDFNode::VelvetBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Velvet BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> sigma_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Sigma", "sigma");
	sigma_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(color_input);
	sockets.push_back(sigma_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::VelvetBSDF;
}

CyclesShaderEditor::ToonBSDFNode::ToonBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Toon BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> comp_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Component", "component");
	StringEnumSocketValue* comp_value = new StringEnumSocketValue();
	comp_value->enum_values.push_back(StringEnumPair("Diffuse", "diffuse"));
	comp_value->enum_values.push_back(StringEnumPair("Glossy", "glossy"));
	comp_value->set_from_internal_name("diffuse");
	comp_input->value = comp_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> size_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Size", "size");
	size_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> smooth_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Smooth", "smooth");
	smooth_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(size_input);
	sockets.push_back(smooth_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::ToonBSDF;
}

CyclesShaderEditor::SubsurfaceScatteringNode::SubsurfaceScatteringNode(FloatPos position)
{
	world_pos = position;

	title = "Subsurface Scattering";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSSRDF", "BSSRDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> comp_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Falloff", "falloff");
	StringEnumSocketValue* comp_value = new StringEnumSocketValue();
	comp_value->enum_values.push_back(StringEnumPair("Christensen-Burley", "burley"));
	comp_value->enum_values.push_back(StringEnumPair("Gaussian", "gaussian"));
	comp_value->enum_values.push_back(StringEnumPair("Cubic", "cubic"));
	comp_value->set_from_internal_name("burley");
	comp_input->value = comp_value;
	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> scale_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Scale", "scale");
	scale_input->value = new FloatSocketValue(1.0f, 0.0f, 1000.0f);
	const std::shared_ptr<NodeSocket> radius_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Radius", "radius");
	radius_input->value = new Float3SocketValue(
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f,
		1.0f, 0.0f, 100.0f);
	radius_input->selectable = true;
	const std::shared_ptr<NodeSocket> blur_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Texture Blur", "texture_blur");
	blur_input->value = new FloatSocketValue(0.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(scale_input);
	sockets.push_back(radius_input);
	sockets.push_back(blur_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::SubsurfaceScattering;

	content_width += 20.0f;
}

CyclesShaderEditor::EmissionNode::EmissionNode(FloatPos position)
{
	world_pos = position;

	title = "Emission";

	const std::shared_ptr<NodeSocket> emission_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Emission", "emission");

	sockets.push_back(emission_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> strength_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Strength", "strength");
	strength_input->value = new FloatSocketValue(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(strength_input);

	type = CyclesNodeType::Emission;
}

CyclesShaderEditor::HairBSDFNode::HairBSDFNode(FloatPos position)
{
	world_pos = position;

	title = "Hair BSDF";

	const std::shared_ptr<NodeSocket> bsdf_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "BSDF", "BSDF");

	sockets.push_back(bsdf_output);

	const std::shared_ptr<NodeSocket> comp_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Component", "component");
	StringEnumSocketValue* comp_value = new StringEnumSocketValue();
	comp_value->enum_values.push_back(StringEnumPair("Transmission", "transmission"));
	comp_value->enum_values.push_back(StringEnumPair("Reflection", "reflection"));
	comp_value->set_from_internal_name("reflection");
	comp_input->value = comp_value;

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> offset_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Offset", "offset");
	offset_input->value = new FloatSocketValue(0.0f, -90.0f, 90.0f);
	const std::shared_ptr<NodeSocket> roughness_u_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "RoughnessU", "roughness_u");
	roughness_u_input->value = new FloatSocketValue(0.1f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> roughness_v_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "RoughnessV", "roughness_v");
	roughness_v_input->value = new FloatSocketValue(1.0f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> tangent_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Vector, "Tangent", "tangent");

	sockets.push_back(comp_input);
	sockets.push_back(color_input);
	sockets.push_back(offset_input);
	sockets.push_back(roughness_u_input);
	sockets.push_back(roughness_v_input);
	sockets.push_back(tangent_input);

	type = CyclesNodeType::HairBSDF;
}

CyclesShaderEditor::HoldoutNode::HoldoutNode(FloatPos position)
{
	world_pos = position;

	title = "Holdout";

	const std::shared_ptr<NodeSocket> holdout_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Holdout", "holdout");

	sockets.push_back(holdout_output);

	type = CyclesNodeType::Holdout;
}

CyclesShaderEditor::VolumeAbsorptionNode::VolumeAbsorptionNode(FloatPos position)
{
	world_pos = position;

	title = "Volume Absorption";

	const std::shared_ptr<NodeSocket> vol_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Volume", "volume");

	sockets.push_back(vol_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> density_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Density", "density");
	density_input->value = new FloatSocketValue(1.0f, 0.0f, 100000.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);

	type = CyclesNodeType::VolAbsorption;
}

CyclesShaderEditor::VolumeScatterNode::VolumeScatterNode(FloatPos position)
{
	world_pos = position;

	title = "Volume Scatter";

	const std::shared_ptr<NodeSocket> vol_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Closure, "Volume", "volume");

	sockets.push_back(vol_output);

	const std::shared_ptr<NodeSocket> color_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Color", "color");
	color_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	const std::shared_ptr<NodeSocket> density_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Density", "density");
	density_input->value = new FloatSocketValue(1.0f, 0.0f, 100000.0f);
	const std::shared_ptr<NodeSocket> anisotropy_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Anisotropy", "anisotropy");
	anisotropy_input->value = new FloatSocketValue(0.0f, -1.0f, 1.0f);

	sockets.push_back(color_input);
	sockets.push_back(density_input);
	sockets.push_back(anisotropy_input);

	type = CyclesNodeType::VolScatter;
}