// This file contains a few functions to enable converting a serialized shader graph to a ccl::ShaderGraph
// It is separate from the editor because these functions depend on Cycles types and any code using them must be linked against Cycles

ccl::ShaderGraph* create_shader_graph(std::string encoded_graph)
{
	CyclesNodeGraph input_graph(encoded_graph);

	std::map<std::string, ccl::ShaderNode*> nodes_by_name;

	ccl::ShaderGraph* cycles_graph = new ccl::ShaderGraph();

	for (OutputNode& node : input_graph.nodes) {
		ccl::ShaderNode* cycles_node = convert_node(node);
		if (cycles_node != nullptr) {
			nodes_by_name[node.name] = cycles_node;
			cycles_graph->add(cycles_node);
		}
		else if (node.type == CyclesNodeType::MaterialOutput) {
			nodes_by_name[node.name] = cycles_graph->output();
		}
	}

	for (OutputConnection& connection : input_graph.connections) {
		if (nodes_by_name.count(connection.source_node) == 0 || nodes_by_name.count(connection.dest_node) == 0) {
			continue;
		}

		ccl::ShaderNode* source_node = nodes_by_name[connection.source_node];
		ccl::ShaderNode* dest_node = nodes_by_name[connection.dest_node];

		cycles_graph->connect(source_node->output(connection.source_socket.c_str()), dest_node->input(connection.dest_socket.c_str()));
	}

	return cycles_graph;
}

ccl::ShaderNode* convert_node(OutputNode& node)
{
	ccl::ShaderNode* cycles_node = nullptr;

	switch (node.type) {
	case CyclesNodeType::AddShader:
	{
		cycles_node = new ccl::AddClosureNode();
		break;
	}
	case CyclesNodeType::AnisotropicBSDF:
	{
		ccl::AnisotropicBsdfNode* aniso_node = new ccl::AnisotropicBsdfNode();
		cycles_node = aniso_node;
		if (node.string_values.count("distribution") == 1) {
			std::string dist = node.string_values["distribution"];
			if (dist == "ashikhmin_shirley") {
				aniso_node->distribution = ccl::ClosureType::CLOSURE_BSDF_ASHIKHMIN_SHIRLEY_ANISO_ID;
			}
			else if (dist == "beckmann") {
				aniso_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_BECKMANN_ANISO_ID;
			}
			else if (dist == "multiscatter_ggx") {
				aniso_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_MULTI_GGX_ANISO_ID;
			}
			else if (dist == "ggx") {
				aniso_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_ANISO_ID;
			}
		}
		if (node.float3_values.count("color") == 1) {
			aniso_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("roughness") == 1) {
			aniso_node->roughness = node.float_values["roughness"];
		}
		if (node.float_values.count("anisotropy") == 1) {
			aniso_node->anisotropy = node.float_values["anisotropy"];
		}
		if (node.float_values.count("rotation") == 1) {
			aniso_node->rotation = node.float_values["rotation"];
		}
		break;
	}
	case CyclesNodeType::CheckerTex:
	{
		ccl::CheckerTextureNode* tex_node = new ccl::CheckerTextureNode();
		cycles_node = tex_node;
		if (node.float3_values.count("color1") == 1) {
			tex_node->color1 = float3_to_ccl_float3(node.float3_values["color1"]);
		}
		if (node.float3_values.count("color2") == 1) {
			tex_node->color2 = float3_to_ccl_float3(node.float3_values["color2"]);
		}
		if (node.float_values.count("scale") == 1) {
			tex_node->scale = node.float_values["scale"];
		}
		break;
	}
	case CyclesNodeType::DiffuseBSDF:
	{
		ccl::DiffuseBsdfNode* diffuse_node = new ccl::DiffuseBsdfNode();
		cycles_node = diffuse_node;
		if (node.float3_values.count("color") == 1) {
			diffuse_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("roughness") == 1) {
			diffuse_node->roughness = node.float_values["roughness"];
		}
		break;
	}
	case CyclesNodeType::Emission:
	{
		ccl::EmissionNode* emission_node = new ccl::EmissionNode();
		cycles_node = emission_node;
		if (node.float3_values.count("color") == 1) {
			emission_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("strength") == 1) {
			emission_node->strength = node.float_values["strength"];
		}
		break;
	}
	case CyclesNodeType::GlassBSDF:
	{
		ccl::GlassBsdfNode* glass_node = new ccl::GlassBsdfNode();
		cycles_node = glass_node;
		if (node.float3_values.count("color") == 1) {
			glass_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.string_values.count("distribution") == 1) {
			std::string dist = node.string_values["distribution"];
			if (dist == "beckmann") {
				glass_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_BECKMANN_GLASS_ID;
			}
			else if (dist == "multiscatter_ggx") {
				glass_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_MULTI_GGX_GLASS_ID;
			}
			else if (dist == "ggx") {
				glass_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_GLASS_ID;
			}
			else if (dist == "sharp") {
				glass_node->distribution = ccl::ClosureType::CLOSURE_BSDF_REFLECTION_ID;
			}
		}
		if (node.float_values.count("roughness") == 1) {
			glass_node->roughness = node.float_values["roughness"];
		}
		if (node.float_values.count("IOR") == 1) {
			glass_node->IOR = node.float_values["IOR"];
		}
		break;
	}
	case CyclesNodeType::GlossyBSDF:
	{
		ccl::GlossyBsdfNode* glossy_node = new ccl::GlossyBsdfNode();
		cycles_node = glossy_node;
		if (node.float3_values.count("color") == 1) {
			glossy_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.string_values.count("distribution") == 1) {
			std::string dist = node.string_values["distribution"];
			if (dist == "ashikhmin_shirley") {
				glossy_node->distribution = ccl::ClosureType::CLOSURE_BSDF_ASHIKHMIN_SHIRLEY_ID;
			}
			else if (dist == "beckmann") {
				glossy_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_BECKMANN_ID;
			}
			else if (dist == "ggx") {
				glossy_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_ID;
			}
			else if (dist == "multiscatter_ggx") {
				glossy_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_MULTI_GGX_ID;
			}
			else if (dist == "sharp") {
				glossy_node->distribution = ccl::ClosureType::CLOSURE_BSDF_REFLECTION_ID;
			}
		}
		if (node.float_values.count("roughness") == 1) {
			glossy_node->roughness = node.float_values["roughness"];
		}
		break;
	}
	case CyclesNodeType::GradientTex:
	{
		ccl::GradientTextureNode* grad_node = new ccl::GradientTextureNode();
		cycles_node = grad_node;
		if (node.string_values.count("type") == 1) {
			std::string type = node.string_values["type"];
			if (type == "linear") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_LINEAR;
			}
			else if (type == "quadratic") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_QUADRATIC;
			}
			else if (type == "easing") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_EASING;
			}
			else if (type == "diagonal") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_DIAGONAL;
			}
			else if (type == "radial") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_RADIAL;
			}
			else if (type == "quadratic_sphere") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_QUADRATIC_SPHERE;
			}
			else if (type == "spherical") {
				grad_node->type = ccl::NodeGradientType::NODE_BLEND_SPHERICAL;
			}
		}
		break;
	}
	case CyclesNodeType::HairBSDF:
	{
		ccl::HairBsdfNode* hair_node = new ccl::HairBsdfNode();
		cycles_node = hair_node;
		if (node.string_values.count("component") == 1) {
			std::string comp = node.string_values["component"];
			if (comp == "transmission") {
				hair_node->component = ccl::ClosureType::CLOSURE_BSDF_HAIR_TRANSMISSION_ID;
			}
			else if (comp == "reflection") {
				hair_node->component = ccl::ClosureType::CLOSURE_BSDF_HAIR_REFLECTION_ID;
			}
		}
		if (node.float3_values.count("color") == 1) {
			hair_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("offset") == 1) {
			hair_node->offset = node.float_values["offset"];
		}
		if (node.float_values.count("roughness_u") == 1) {
			hair_node->roughness_u = node.float_values["roughness_u"];
		}
		if (node.float_values.count("roughness_v") == 1) {
			hair_node->roughness_v = node.float_values["roughness_v"];
		}
		break;
	}
	case CyclesNodeType::Holdout:
	{
		cycles_node = new ccl::HoldoutNode();
		break;
	}
	case CyclesNodeType::MagicTex:
	{
		ccl::MagicTextureNode* magic_node = new ccl::MagicTextureNode();
		cycles_node = magic_node;
		if (node.int_values.count("depth") == 1) {
			magic_node->depth = node.int_values["depth"];
		}
		if (node.float_values.count("scale") == 1) {
			magic_node->scale = node.float_values["scale"];
		}
		if (node.float_values.count("distortion") == 1) {
			magic_node->distortion = node.float_values["distortion"];
		}
		break;
	}
	case CyclesNodeType::MixShader:
	{
		ccl::MixClosureNode* mix_node = new ccl::MixClosureNode();
		cycles_node = mix_node;
		if (node.float_values.count("fac") == 1) {
			mix_node->fac = node.float_values["fac"];
		}
		break;
	}
	case CyclesNodeType::MusgraveTex:
	{
		ccl::MusgraveTextureNode* musgrave_node = new ccl::MusgraveTextureNode();
		cycles_node = musgrave_node;
		if (node.string_values.count("type") == 1) {
			std::string type = node.string_values["type"];
			if (type == "fBM") {
				musgrave_node->type = ccl::NodeMusgraveType::NODE_MUSGRAVE_FBM;
			}
			else if (type == "hetero_terrain") {
				musgrave_node->type = ccl::NodeMusgraveType::NODE_MUSGRAVE_HETERO_TERRAIN;
			}
			else if (type == "hybrid_multifractal") {
				musgrave_node->type = ccl::NodeMusgraveType::NODE_MUSGRAVE_HYBRID_MULTIFRACTAL;
			}
			else if (type == "multifractal") {
				musgrave_node->type = ccl::NodeMusgraveType::NODE_MUSGRAVE_MULTIFRACTAL;
			}
			else if (type == "ridged_multifractal") {
				musgrave_node->type = ccl::NodeMusgraveType::NODE_MUSGRAVE_RIDGED_MULTIFRACTAL;
			}
		}
		if (node.float_values.count("scale") == 1) {
			musgrave_node->scale = node.float_values["scale"];
		}
		if (node.float_values.count("detail") == 1) {
			musgrave_node->detail = node.float_values["detail"];
		}
		if (node.float_values.count("dimension") == 1) {
			musgrave_node->dimension = node.float_values["dimension"];
		}
		if (node.float_values.count("lacunarity") == 1) {
			musgrave_node->lacunarity = node.float_values["lacunarity"];
		}
		if (node.float_values.count("offset") == 1) {
			musgrave_node->offset = node.float_values["offset"];
		}
		if (node.float_values.count("gain") == 1) {
			musgrave_node->gain = node.float_values["gain"];
		}
		break;
	}
	case CyclesNodeType::NoiseTex:
	{
		ccl::NoiseTextureNode* noise_node = new ccl::NoiseTextureNode();
		cycles_node = noise_node;
		if (node.float_values.count("scale") == 1) {
			noise_node->scale = node.float_values["scale"];
		}
		if (node.float_values.count("detail") == 1) {
			noise_node->detail = node.float_values["detail"];
		}
		if (node.float_values.count("distortion") == 1) {
			noise_node->distortion = node.float_values["distortion"];
		}
		break;
	}
	case CyclesNodeType::PrincipledBSDF:
	{
		ccl::PrincipledBsdfNode* principled_node = new ccl::PrincipledBsdfNode();
		cycles_node = principled_node;
		if (node.string_values.count("distribution") == 1) {
			std::string dist = node.string_values["distribution"];
			if (dist == "ggx") {
				principled_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_GLASS_ID;
			}
			else if (dist == "multiscatter_ggx") {
				principled_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_MULTI_GGX_GLASS_ID;
			}
		}
		if (node.float3_values.count("base_color") == 1) {
			principled_node->base_color = float3_to_ccl_float3(node.float3_values["base_color"]);
		}
		if (node.float_values.count("subsurface") == 1) {
			principled_node->subsurface = node.float_values["subsurface"];
		}
		if (node.float3_values.count("subsurface_radius") == 1) {
			principled_node->subsurface_radius = float3_to_ccl_float3(node.float3_values["subsurface_radius"]);
		}
		if (node.float3_values.count("subsurface_color") == 1) {
			principled_node->subsurface_color = float3_to_ccl_float3(node.float3_values["subsurface_color"]);
		}
		if (node.float_values.count("metallic") == 1) {
			principled_node->metallic = node.float_values["metallic"];
		}
		if (node.float_values.count("specular") == 1) {
			principled_node->specular = node.float_values["specular"];
		}
		if (node.float_values.count("specular_tint") == 1) {
			principled_node->specular_tint = node.float_values["specular_tint"];
		}
		if (node.float_values.count("roughness") == 1) {
			principled_node->roughness = node.float_values["roughness"];
		}
		if (node.float_values.count("anisotropic") == 1) {
			principled_node->anisotropic = node.float_values["anisotropic"];
		}
		if (node.float_values.count("anisotropic_rotation") == 1) {
			principled_node->anisotropic_rotation = node.float_values["anisotropic_rotation"];
		}
		if (node.float_values.count("sheen") == 1) {
			principled_node->sheen = node.float_values["sheen"];
		}
		if (node.float_values.count("sheen_tint") == 1) {
			principled_node->sheen_tint = node.float_values["sheen_tint"];
		}
		if (node.float_values.count("clearcoat") == 1) {
			principled_node->clearcoat = node.float_values["clearcoat"];
		}
		if (node.float_values.count("clearcoat_roughness") == 1) {
			principled_node->clearcoat_roughness = node.float_values["clearcoat_roughness"];
		}
		if (node.float_values.count("ior") == 1) {
			principled_node->ior = node.float_values["ior"];
		}
		if (node.float_values.count("transmission") == 1) {
			principled_node->transmission = node.float_values["transmission"];
		}
		break;
	}
	case CyclesNodeType::RefractionBSDF:
	{
		ccl::RefractionBsdfNode* refraction_node = new ccl::RefractionBsdfNode();
		cycles_node = refraction_node;
		if (node.string_values.count("distribution") == 1) {
			std::string dist = node.string_values["distribution"];
			if (dist == "ggx") {
				refraction_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_GGX_REFRACTION_ID;
			}
			else if (dist == "beckmann") {
				refraction_node->distribution = ccl::ClosureType::CLOSURE_BSDF_MICROFACET_BECKMANN_REFRACTION_ID;
			}
			else if (dist == "sharp") {
				refraction_node->distribution = ccl::ClosureType::CLOSURE_BSDF_REFRACTION_ID;
			}
		}
		if (node.float3_values.count("color") == 1) {
			refraction_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("roughness") == 1) {
			refraction_node->roughness = node.float_values["roughness"];
		}
		if (node.float_values.count("IOR") == 1) {
			refraction_node->IOR = node.float_values["IOR"];
		}
		break;
	}
	case CyclesNodeType::SubsurfaceScattering:
	{
		ccl::SubsurfaceScatteringNode* sss_node = new ccl::SubsurfaceScatteringNode();
		cycles_node = sss_node;
		if (node.string_values.count("falloff") == 1) {
			std::string falloff = node.string_values["falloff"];
			if (falloff == "burley") {
				sss_node->falloff = ccl::ClosureType::CLOSURE_BSSRDF_BURLEY_ID;
			}
			else if (falloff == "gaussian") {
				sss_node->falloff = ccl::ClosureType::CLOSURE_BSSRDF_GAUSSIAN_ID;
			}
			else if (falloff == "cubic") {
				sss_node->falloff = ccl::ClosureType::CLOSURE_BSSRDF_CUBIC_ID;
			}
		}
		if (node.float3_values.count("color") == 1) {
			sss_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("scale") == 1) {
			sss_node->scale = node.float_values["scale"];
		}
		if (node.float3_values.count("radius") == 1) {
			sss_node->radius = float3_to_ccl_float3(node.float3_values["radius"]);
		}
		if (node.float_values.count("texture_blur") == 1) {
			sss_node->texture_blur = node.float_values["texture_blur"];
		}
		break;
	}
	case CyclesNodeType::Tangent:
	{
		ccl::TangentNode* tangent_node = new ccl::TangentNode();
		tangent_node->attribute = "";
		cycles_node = tangent_node;
		if (node.string_values.count("direction") == 1) {
			std::string dir = node.string_values["direction"];
			if (dir == "uv_map") {
				tangent_node->direction_type = ccl::NodeTangentDirectionType::NODE_TANGENT_UVMAP;
			}
			else {
				tangent_node->direction_type = ccl::NodeTangentDirectionType::NODE_TANGENT_RADIAL;
			}
		}
		if (node.string_values.count("axis") == 1) {
			std::string axis = node.string_values["axis"];
			if (axis == "x") {
				tangent_node->axis = ccl::NodeTangentAxis::NODE_TANGENT_AXIS_X;
			}
			else if (axis == "y") {
				tangent_node->axis = ccl::NodeTangentAxis::NODE_TANGENT_AXIS_Y;
			}
			else {
				tangent_node->axis = ccl::NodeTangentAxis::NODE_TANGENT_AXIS_Z;
			}
		}
		break;
	}
	case CyclesNodeType::TextureCoordinate:
	{
		cycles_node = new ccl::TextureCoordinateNode();
		break;
	}
	case CyclesNodeType::ToonBSDF:
	{
		ccl::ToonBsdfNode* toon_node = new ccl::ToonBsdfNode();
		cycles_node = toon_node;
		if (node.string_values.count("component") == 1) {
			std::string comp = node.string_values["component"];
			if (comp == "diffuse") {
				toon_node->component = ccl::ClosureType::CLOSURE_BSDF_DIFFUSE_TOON_ID;
			}
			else if (comp == "glossy") {
				toon_node->component = ccl::ClosureType::CLOSURE_BSDF_GLOSSY_TOON_ID;
			}
		}
		if (node.float3_values.count("color") == 1) {
			toon_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("size") == 1) {
			toon_node->size = node.float_values["size"];
		}
		if (node.float_values.count("smooth") == 1) {
			toon_node->smooth = node.float_values["smooth"];
		}
		break;
	}
	case CyclesNodeType::TranslucentBSDF:
	{
		ccl::TranslucentBsdfNode* translucent_node = new ccl::TranslucentBsdfNode();
		cycles_node = translucent_node;
		if (node.float3_values.count("color") == 1) {
			translucent_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		break;
	}
	case CyclesNodeType::TransparentBSDF:
	{
		ccl::TransparentBsdfNode* transparent_node = new ccl::TransparentBsdfNode();
		cycles_node = transparent_node;
		if (node.float3_values.count("color") == 1) {
			transparent_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		break;
	}
	case CyclesNodeType::VelvetBSDF:
	{
		ccl::VelvetBsdfNode* velvet_node = new ccl::VelvetBsdfNode();
		cycles_node = velvet_node;
		if (node.float3_values.count("color") == 1) {
			velvet_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("sigma") == 1) {
			velvet_node->sigma = node.float_values["sigma"];
		}
		break;
	}
	case CyclesNodeType::VolAbsorption:
	{
		ccl::AbsorptionVolumeNode* absorb_node = new ccl::AbsorptionVolumeNode();
		cycles_node = absorb_node;
		if (node.float3_values.count("color") == 1) {
			absorb_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("density") == 1) {
			absorb_node->density = node.float_values["density"];
		}
		break;
	}
	case CyclesNodeType::VolScatter:
	{
		ccl::ScatterVolumeNode* scatter_node = new ccl::ScatterVolumeNode();
		cycles_node = scatter_node;
		if (node.float3_values.count("color") == 1) {
			scatter_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("density") == 1) {
			scatter_node->density = node.float_values["density"];
		}
		if (node.float_values.count("anisotropy") == 1) {
			scatter_node->anisotropy = node.float_values["anisotropy"];
		}
		break;
	}
	case CyclesNodeType::VoronoiTex:
	{
		ccl::VoronoiTextureNode* voronoi_node = new ccl::VoronoiTextureNode();
		cycles_node = voronoi_node;
		if (node.string_values.count("coloring") == 1) {
			std::string coloring = node.string_values["coloring"];
			if (coloring == "cells") {
				voronoi_node->coloring = ccl::NodeVoronoiColoring::NODE_VORONOI_CELLS;
			}
			else if (coloring == "intensity") {
				voronoi_node->coloring = ccl::NodeVoronoiColoring::NODE_VORONOI_INTENSITY;
			}
		}
		if (node.float_values.count("scale") == 1) {
			voronoi_node->scale = node.float_values["scale"];
		}
		break;
	}
	case CyclesNodeType::WaveTex:
	{
		ccl::WaveTextureNode* wave_node = new ccl::WaveTextureNode();
		cycles_node = wave_node;
		if (node.string_values.count("type") == 1) {
			std::string type = node.string_values["type"];
			if (type == "bands") {
				wave_node->type = ccl::NodeWaveType::NODE_WAVE_BANDS;
			}
			else if (type == "rings") {
				wave_node->type = ccl::NodeWaveType::NODE_WAVE_RINGS;
			}
		}
		if (node.string_values.count("profile") == 1) {
			std::string profile = node.string_values["profile"];
			if (profile == "saw") {
				wave_node->profile = ccl::NodeWaveProfile::NODE_WAVE_PROFILE_SAW;
			}
			else if (profile == "sine") {
				wave_node->profile = ccl::NodeWaveProfile::NODE_WAVE_PROFILE_SIN;
			}
		}
		if (node.float_values.count("scale") == 1) {
			wave_node->scale = node.float_values["scale"];
		}
		if (node.float_values.count("distortion") == 1) {
			wave_node->distortion = node.float_values["distortion"];
		}
		if (node.float_values.count("detail") == 1) {
			wave_node->detail = node.float_values["detail"];
		}
		if (node.float_values.count("detail_scale") == 1) {
			wave_node->detail_scale = node.float_values["detail_scale"];
		}
		break;
	}
	case CyclesNodeType::LightPath:
	{
		cycles_node = new ccl::LightPathNode();
		break;
	}
	case CyclesNodeType::Fresnel:
	{
		ccl::FresnelNode* fresnel_node = new ccl::FresnelNode();
		cycles_node = fresnel_node;
		if (node.float_values.count("IOR") == 1) {
			fresnel_node->IOR = node.float_values["IOR"];
		}
		break;
	}
	case CyclesNodeType::LayerWeight:
	{
		ccl::LayerWeightNode* lw_node = new ccl::LayerWeightNode();
		cycles_node = lw_node;
		if (node.float_values.count("blend") == 1) {
			lw_node->blend = node.float_values["blend"];
		}
		break;
	}
	case CyclesNodeType::CameraData:
	{
		cycles_node = new ccl::CameraNode();
		break;
	}
	case CyclesNodeType::MixRGB:
	{
		ccl::MixNode* mix_node = new ccl::MixNode();
		cycles_node = mix_node;
		if (node.string_values.count("type") == 1) {
			std::string type = node.string_values["type"];
			if (type == "linear_light") {
				mix_node->type = ccl::NodeMix::NODE_MIX_LINEAR;
			}
			else if (type == "soft_light") {
				mix_node->type = ccl::NodeMix::NODE_MIX_SOFT;
			}
			else if (type == "color") {
				mix_node->type = ccl::NodeMix::NODE_MIX_COLOR;
			}
			else if (type == "value") {
				mix_node->type = ccl::NodeMix::NODE_MIX_VAL;
			}
			else if (type == "saturation") {
				mix_node->type = ccl::NodeMix::NODE_MIX_SAT;
			}
			else if (type == "hue") {
				mix_node->type = ccl::NodeMix::NODE_MIX_HUE;
			}
			else if (type == "burn") {
				mix_node->type = ccl::NodeMix::NODE_MIX_BURN;
			}
			else if (type == "dodge") {
				mix_node->type = ccl::NodeMix::NODE_MIX_DODGE;
			}
			else if (type == "overlay") {
				mix_node->type = ccl::NodeMix::NODE_MIX_OVERLAY;
			}
			else if (type == "lighten") {
				mix_node->type = ccl::NodeMix::NODE_MIX_LIGHT;
			}
			else if (type == "darken") {
				mix_node->type = ccl::NodeMix::NODE_MIX_DARK;
			}
			else if (type == "difference") {
				mix_node->type = ccl::NodeMix::NODE_MIX_DIFF;
			}
			else if (type == "divide") {
				mix_node->type = ccl::NodeMix::NODE_MIX_DIV;
			}
			else if (type == "screen") {
				mix_node->type = ccl::NodeMix::NODE_MIX_SCREEN;
			}
			else if (type == "subtract") {
				mix_node->type = ccl::NodeMix::NODE_MIX_SUB;
			}
			else if (type == "multiply") {
				mix_node->type = ccl::NodeMix::NODE_MIX_MUL;
			}
			else if (type == "add") {
				mix_node->type = ccl::NodeMix::NODE_MIX_ADD;
			}
			else if (type == "mix") {
				mix_node->type = ccl::NodeMix::NODE_MIX_BLEND;
			}
		}
		if (node.bool_values.count("use_clamp") == 1) {
			mix_node->use_clamp = node.bool_values["use_clamp"];
		}
		if (node.float_values.count("fac") == 1) {
			mix_node->fac = node.float_values["fac"];
		}
		if (node.float3_values.count("color1") == 1) {
			mix_node->color1 = float3_to_ccl_float3(node.float3_values["color1"]);
		}
		if (node.float3_values.count("color2") == 1) {
			mix_node->color2 = float3_to_ccl_float3(node.float3_values["color2"]);
		}
		break;
	}
	case CyclesNodeType::Invert:
	{
		ccl::InvertNode* invert_node = new ccl::InvertNode();
		cycles_node = invert_node;
		if (node.float_values.count("fac") == 1) {
			invert_node->fac = node.float_values["fac"];
		}
		if (node.float3_values.count("color") == 1) {
			invert_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		break;
	}
	case CyclesNodeType::LightFalloff:
	{
		ccl::LightFalloffNode* falloff_node = new ccl::LightFalloffNode();
		cycles_node = falloff_node;
		if (node.float_values.count("strength") == 1) {
			falloff_node->strength = node.float_values["strength"];
		}
		if (node.float_values.count("smooth") == 1) {
			falloff_node->smooth = node.float_values["smooth"];
		}
		break;
	}
	case CyclesNodeType::HSV:
	{
		ccl::HSVNode* hsv_node = new ccl::HSVNode();
		cycles_node = hsv_node;
		if (node.float_values.count("hue") == 1) {
			hsv_node->hue = node.float_values["hue"];
		}
		if (node.float_values.count("saturation") == 1) {
			hsv_node->saturation = node.float_values["saturation"];
		}
		if (node.float_values.count("value") == 1) {
			hsv_node->value = node.float_values["value"];
		}
		if (node.float_values.count("fac") == 1) {
			hsv_node->fac = node.float_values["fac"];
		}
		if (node.float3_values.count("color") == 1) {
			hsv_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		break;
	}
	case CyclesNodeType::Gamma:
	{
		ccl::GammaNode* gamma_node = new ccl::GammaNode();
		cycles_node = gamma_node;
		if (node.float3_values.count("color") == 1) {
			gamma_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("gamma") == 1) {
			gamma_node->gamma = node.float_values["gamma"];
		}
		break;
	}
	case CyclesNodeType::BrightnessContrast:
	{
		ccl::BrightContrastNode* bc_node = new ccl::BrightContrastNode();
		cycles_node = bc_node;
		if (node.float3_values.count("color") == 1) {
			bc_node->color = float3_to_ccl_float3(node.float3_values["color"]);
		}
		if (node.float_values.count("bright") == 1) {
			bc_node->bright = node.float_values["bright"];
		}
		if (node.float_values.count("contrast") == 1) {
			bc_node->contrast = node.float_values["contrast"];
		}
		break;
	}
	case CyclesNodeType::Bump:
	{
		ccl::BumpNode* bump_node = new ccl::BumpNode();
		cycles_node = bump_node;
		if (node.bool_values.count("invert") == 1) {
			bump_node->invert = node.bool_values["invert"];
		}
		if (node.float_values.count("strength") == 1) {
			bump_node->strength = node.float_values["strength"];
		}
		if (node.float_values.count("distance") == 1) {
			bump_node->distance = node.float_values["distance"];
		}
		if (node.float_values.count("height") == 1) {
			bump_node->height = node.float_values["height"];
		}
		break;
	}
	case CyclesNodeType::NormalMap:
	{
		ccl::NormalMapNode* normal_node = new ccl::NormalMapNode();
		normal_node->attribute = "";
		cycles_node = normal_node;
		if (node.string_values.count("space") == 1) {
			std::string space = node.string_values["space"];
			if (space == "tangent") {
				normal_node->space = ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_TANGENT;
			}
			else if (space == "object") {
				normal_node->space = ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_OBJECT;
			}
			else if (space == "world") {
				normal_node->space = ccl::NodeNormalMapSpace::NODE_NORMAL_MAP_WORLD;
			}
		}
		break;
	}
	default:
		break;
	}

	return cycles_node;
}
