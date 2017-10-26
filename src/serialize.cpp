#include "serialize.h"

#include <cassert>
#include <map>
#include <sstream>

#include "node_colors.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_outputs.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"
#include "output.h"

#include <iostream>

static const char SEPARATOR = '|';

static const char* MAGIC_WORD = "cycles_shader";
static const char* CURRENT_VERSION = "1";

static const char* SECTION_LABEL_NODE = "section_nodes";
static const char* SECTION_LABEL_CONNECTION = "section_connections";

static const char* NODE_END = "node_end";

std::map<CyclesShaderEditor::CyclesNodeType, std::string> type_to_code;
std::map<std::string, CyclesShaderEditor::CyclesNodeType> code_to_type;

static std::string create_node_name()
{
	static int number = 0;
	return std::string("node") + std::to_string(number++);
}

static void initialize_maps()
{
	using namespace CyclesShaderEditor;

	if (!type_to_code.empty()) {
		return;
	}

	type_to_code[CyclesNodeType::PrincipledBSDF] = std::string("principled_bsdf");
	type_to_code[CyclesNodeType::MixShader] = std::string("mix_shader");
	type_to_code[CyclesNodeType::AddShader] = std::string("add_shader");
	type_to_code[CyclesNodeType::DiffuseBSDF] = std::string("diffuse_bsdf");
	type_to_code[CyclesNodeType::GlossyBSDF] = std::string("glossy_bsdf");
	type_to_code[CyclesNodeType::TransparentBSDF] = std::string("transparent_bsdf");
	type_to_code[CyclesNodeType::RefractionBSDF] = std::string("refraction_bsdf");
	type_to_code[CyclesNodeType::GlassBSDF] = std::string("glass_bsdf");
	type_to_code[CyclesNodeType::TranslucentBSDF] = std::string("translucent_bsdf");
	type_to_code[CyclesNodeType::AnisotropicBSDF] = std::string("anisotropic_bsdf");
	type_to_code[CyclesNodeType::VelvetBSDF] = std::string("velvet_bsdf");
	type_to_code[CyclesNodeType::ToonBSDF] = std::string("toon_bsdf");
	type_to_code[CyclesNodeType::SubsurfaceScattering] = std::string("subsurface_scatter");
	type_to_code[CyclesNodeType::Emission] = std::string("emission");
	type_to_code[CyclesNodeType::HairBSDF] = std::string("hair_bsdf");
	type_to_code[CyclesNodeType::Holdout] = std::string("holdout");
	type_to_code[CyclesNodeType::VolAbsorption] = std::string("vol_absorb");
	type_to_code[CyclesNodeType::VolScatter] = std::string("vol_scatter");

	type_to_code[CyclesNodeType::NoiseTex] = std::string("noise_tex");
	type_to_code[CyclesNodeType::WaveTex] = std::string("wave_tex");
	type_to_code[CyclesNodeType::VoronoiTex] = std::string("voronoi_tex");
	type_to_code[CyclesNodeType::MusgraveTex] = std::string("musgrave_tex");
	type_to_code[CyclesNodeType::GradientTex] = std::string("gradient_tex");
	type_to_code[CyclesNodeType::MagicTex] = std::string("magix_tex");
	type_to_code[CyclesNodeType::CheckerTex] = std::string("checker_tex");
	type_to_code[CyclesNodeType::MaxTex] = std::string("max_tex");

	type_to_code[CyclesNodeType::LightPath] = std::string("light_path");
	type_to_code[CyclesNodeType::Fresnel] = std::string("fresnel");
	type_to_code[CyclesNodeType::LayerWeight] = std::string("layer_weight");
	type_to_code[CyclesNodeType::CameraData] = std::string("camera_data");
	type_to_code[CyclesNodeType::Tangent] = std::string("tangent");
	type_to_code[CyclesNodeType::TextureCoordinate] = std::string("texture_coordinate");

	type_to_code[CyclesNodeType::MixRGB] = std::string("mix_rgb");
	type_to_code[CyclesNodeType::Invert] = std::string("invert");
	type_to_code[CyclesNodeType::LightFalloff] = std::string("light_falloff");
	type_to_code[CyclesNodeType::HSV] = std::string("hsv");
	type_to_code[CyclesNodeType::Gamma] = std::string("gamma");
	type_to_code[CyclesNodeType::BrightnessContrast] = std::string("bright_contrast");

	type_to_code[CyclesNodeType::Bump] = std::string("bump");
	type_to_code[CyclesNodeType::NormalMap] = std::string("normal_map");

	type_to_code[CyclesNodeType::MaterialOutput] = std::string("out_material");

	for (std::pair<CyclesNodeType, std::string> this_pair : type_to_code) {
		code_to_type[this_pair.second] = this_pair.first;
	}

	assert(code_to_type.size() == type_to_code.size());
}

static std::string serialize_node(const CyclesShaderEditor::OutputNode& node)
{
	using namespace CyclesShaderEditor;

	if (type_to_code.count(node.type) == 0) {
		return std::string();
	}

	std::stringstream node_stream;

	node_stream << type_to_code[node.type] << SEPARATOR << node.name << SEPARATOR << node.world_x << SEPARATOR << node.world_y << SEPARATOR;

	for (std::pair<std::string, float> this_pair : node.float_values) {
		node_stream << this_pair.first << SEPARATOR << this_pair.second << SEPARATOR;
	}
	for (std::pair<std::string, Float3> this_pair : node.float3_values) {
		node_stream << this_pair.first << SEPARATOR
			<< this_pair.second.x << ',' << this_pair.second.y << ',' << this_pair.second.z << SEPARATOR;
	}
	for (std::pair<std::string, std::string> this_pair : node.string_values) {
		node_stream << this_pair.first << SEPARATOR << this_pair.second << SEPARATOR;
	}
	for (std::pair<std::string, int> this_pair : node.int_values) {
		node_stream << this_pair.first << SEPARATOR << this_pair.second << SEPARATOR;
	}
	for (std::pair<std::string, bool> this_pair : node.bool_values) {
		node_stream << this_pair.first << SEPARATOR << static_cast<int>(this_pair.second) << SEPARATOR;
	}

	node_stream << NODE_END << SEPARATOR;

	return node_stream.str();
}

static std::string serialize_connection(const CyclesShaderEditor::OutputConnection& connection)
{
	std::stringstream connection_stream;
	connection_stream << connection.source_node << SEPARATOR << connection.source_socket << SEPARATOR << connection.dest_node << SEPARATOR << connection.dest_socket << SEPARATOR;
	return connection_stream.str();
}

void CyclesShaderEditor::generate_output_lists(std::list<EditorNode*>& node_list, std::list<NodeConnection>& connection_list, std::vector<OutputNode>& out_node_list, std::vector<OutputConnection>& out_connection_list)
{
	using namespace CyclesShaderEditor;

	std::map<EditorNode*, std::string> node_to_name_map;

	for (EditorNode* this_node : node_list) {
		OutputNode this_out_node;
		this_out_node.name = create_node_name();

		this_node->update_output_node(this_out_node);

		out_node_list.push_back(this_out_node);
		node_to_name_map[this_node] = this_out_node.name;
	}

	for (NodeConnection this_connection : connection_list) {
		OutputConnection this_out_connection;
		this_out_connection.source_node = node_to_name_map[this_connection.begin_socket->parent];
		this_out_connection.dest_node = node_to_name_map[this_connection.end_socket->parent];
		this_out_connection.source_socket = this_connection.begin_socket->display_name;
		this_out_connection.dest_socket = this_connection.end_socket->display_name;
		out_connection_list.push_back(this_out_connection);
	}
}

std::string CyclesShaderEditor::serialize_graph(std::vector<OutputNode> &nodes, std::vector<OutputConnection> &connections)
{
	initialize_maps();

	std::stringstream output_stream;

	// Write first entry
	output_stream << MAGIC_WORD << SEPARATOR << CURRENT_VERSION << SEPARATOR;

	// Fill in node information
	output_stream << SECTION_LABEL_NODE << SEPARATOR;
	for (OutputNode node : nodes) {
		output_stream << serialize_node(node);
	}

	// Fill in connection information
	output_stream << SECTION_LABEL_CONNECTION << SEPARATOR;
	for (OutputConnection connection : connections) {
		output_stream << serialize_connection(connection);
	}

	return output_stream.str();
}

static std::list<std::string> tokenize_string(std::string input, char delim)
{
	std::list<std::string> output;

	std::string::size_type index = 0;
	std::string::size_type next_delim = input.find(delim);
	while (next_delim != std::string::npos) {
		output.push_back(input.substr(index, next_delim - index));
		index = next_delim + 1;
		next_delim = input.find(delim, index);

		if (next_delim == std::string::npos) {
			output.push_back(input.substr(index, input.length()));
		}
	}

	return output;
}

static CyclesShaderEditor::EditorNode* deserialize_node(std::list<std::string>& tokens, std::map<std::string, CyclesShaderEditor::EditorNode*>& nodes_by_name)
{
	using namespace CyclesShaderEditor;

	initialize_maps();

	if (tokens.size() < 4) {
		return nullptr;
	}

	std::list<std::string>::iterator token_iter = tokens.begin();

	std::string type_code = *(token_iter++);
	std::string name = *(token_iter++);
	float x_position = std::stof(*(token_iter++));
	float y_position = std::stof(*(token_iter++));

	std::map<std::string, std::string> params;
	while (token_iter != tokens.end()) {
		std::string name = *(token_iter++);
		if (token_iter == tokens.end()) {
			break;
		}
		std::string value = *(token_iter++);
		params[name] = value;
	}

	if (code_to_type.count(type_code) == 0) {
		// Unknown type
		return nullptr;
	}

	CyclesNodeType type = code_to_type[type_code];

	EditorNode* result = nullptr;
	switch (type) {
	case CyclesNodeType::PrincipledBSDF:
	{
		PrincipledBSDFNode* node = new PrincipledBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MixShader:
	{
		MixShaderNode* node = new MixShaderNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::AddShader:
	{
		AddShaderNode* node = new AddShaderNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::DiffuseBSDF:
	{
		DiffuseBSDFNode* node = new DiffuseBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::GlossyBSDF:
	{
		GlossyBSDFNode* node = new GlossyBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::TransparentBSDF:
	{
		TransparentBSDFNode* node = new TransparentBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::RefractionBSDF:
	{
		RefractionBSDFNode* node = new RefractionBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::GlassBSDF:
	{
		GlassBSDFNode* node = new GlassBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::TranslucentBSDF:
	{
		TranslucentBSDFNode* node = new TranslucentBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::AnisotropicBSDF:
	{
		AnisotropicBSDFNode* node = new AnisotropicBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::VelvetBSDF:
	{
		VelvetBSDFNode* node = new VelvetBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::ToonBSDF:
	{
		ToonBSDFNode* node = new ToonBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::SubsurfaceScattering:
	{
		SubsurfaceScatteringNode* node = new SubsurfaceScatteringNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Emission:
	{
		EmissionNode* node = new EmissionNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::HairBSDF:
	{
		HairBSDFNode* node = new HairBSDFNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Holdout:
	{
		HoldoutNode* node = new HoldoutNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::VolAbsorption:
	{
		VolumeAbsorptionNode* node = new VolumeAbsorptionNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::VolScatter:
	{
		VolumeScatterNode* node = new VolumeScatterNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::NoiseTex:
	{
		NoiseTextureNode* node = new NoiseTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::WaveTex:
	{
		WaveTextureNode* node = new WaveTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::VoronoiTex:
	{
		VoronoiTextureNode* node = new VoronoiTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MusgraveTex:
	{
		MusgraveTextureNode* node = new MusgraveTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::GradientTex:
	{
		GradientTextureNode* node = new GradientTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MagicTex:
	{
		MagicTextureNode* node = new MagicTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::CheckerTex:
	{
		CheckerTextureNode* node = new CheckerTextureNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MaxTex:
	{
		MaxTexmapShaderNode* node = new MaxTexmapShaderNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::LightPath:
	{
		LightPathNode* node = new LightPathNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Fresnel:
	{
		FresnelNode* node = new FresnelNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::LayerWeight:
	{
		LayerWeightNode* node = new LayerWeightNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::CameraData:
	{
		CameraDataNode* node = new CameraDataNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Tangent:
	{
		TangentNode* node = new TangentNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::TextureCoordinate:
	{
		TextureCoordinateNode* node = new TextureCoordinateNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MixRGB:
	{
		MixRGBNode* node = new MixRGBNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Invert:
	{
		InvertNode* node = new InvertNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::LightFalloff:
	{
		LightFalloffNode* node = new LightFalloffNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::HSV:
	{
		HSVNode* node = new HSVNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Gamma:
	{
		GammaNode* node = new GammaNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::BrightnessContrast:
	{
		BrightnessContrastNode* node = new BrightnessContrastNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::Bump:
	{
		BumpNode* node = new BumpNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::NormalMap:
	{
		NormalMapNode* node = new NormalMapNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	case CyclesNodeType::MaterialOutput:
	{
		MaterialOutputNode* node = new MaterialOutputNode(CyclesShaderEditor::Point2(x_position, y_position));
		result = node;
		break;
	}
	default:
		return nullptr;
	}
	
	if (result == nullptr) {
		return nullptr;
	}

	for (std::pair<std::string, std::string> this_param : params) {
		NodeSocket* this_socket = result->get_socket_by_internal_name(SocketInOut::Input, this_param.first);
		switch (this_socket->socket_type) {

		case SocketType::Float:
			this_socket->set_float_val(std::stof(this_param.second));
			break;

		case SocketType::Color:
		case SocketType::Vector:
		{
			std::list<std::string> floats = tokenize_string(this_param.second, ',');
			if (floats.size() != 3) {
				break;
			}
			std::list<std::string>::iterator float_iter = floats.begin();
			std::string x = *(float_iter++);
			std::string y = *(float_iter++);
			std::string z = *(float_iter++);
			this_socket->set_float3_val(std::stof(x), std::stof(y), std::stof(z));
			break;
		}

		case SocketType::StringEnum:
			if (this_socket->value != nullptr) {
				dynamic_cast<StringEnumSocketValue*>(this_socket->value)->set_from_internal_name(this_param.second);
			}
			break;

		case SocketType::Int:
			if (this_socket->value != nullptr) {
				IntSocketValue* int_val = dynamic_cast<IntSocketValue*>(this_socket->value);
				int_val->set_value(std::stoi(this_param.second));
			}
			break;

		case SocketType::Boolean:
			if (this_socket->value != nullptr) {
				BoolSocketValue* bool_val = dynamic_cast<BoolSocketValue*>(this_socket->value);
				bool_val->value = std::stoi(this_param.second) != 0;
			}
			break;

		default:
			break;
		}
	}

	nodes_by_name[name] = result;

	return result;
}

void CyclesShaderEditor::deserialize_graph(std::string graph, std::list<EditorNode*>& nodes, std::list<NodeConnection>& connections)
{
	std::map<std::string, EditorNode*> nodes_by_name;

	std::list<std::string> graph_tokens = tokenize_string(graph, SEPARATOR);

	std::list<std::string>::iterator token_iter = graph_tokens.begin();

	if (token_iter == graph_tokens.end() || (*token_iter) != MAGIC_WORD) {
		return;
	}
	++token_iter;

	if (token_iter == graph_tokens.end() || (*token_iter) != CURRENT_VERSION) {
		return;
	}
	++token_iter;

	// Construct nodes
	if (token_iter == graph_tokens.end() || (*token_iter) != SECTION_LABEL_NODE) {
		return;
	}
	++token_iter;

	// Loop making nodes until we see connection section
	while (true) {
		std::list<std::string> node_tokens;
		while (token_iter != graph_tokens.end() && (*token_iter) != NODE_END) {
			node_tokens.push_back(*token_iter);
			++token_iter;
		}

		EditorNode* node = deserialize_node(node_tokens, nodes_by_name);
		if (node != nullptr) {
			nodes.push_back(node);
		}

		if (token_iter == graph_tokens.end()) {
			return;
		}

		++token_iter;
		if (token_iter == graph_tokens.end() || *token_iter == SECTION_LABEL_CONNECTION) {
			break;
		}
	}

	// Advance past connection begin token
	++token_iter;

	// Loop while making connections
	while (token_iter != graph_tokens.end()) {

		if (token_iter == graph_tokens.end()) {
			break;
		}
		std::string source_node = *(token_iter++);
		if (token_iter == graph_tokens.end()) {
			break;
		}
		std::string source_socket = *(token_iter++);
		if (token_iter == graph_tokens.end()) {
			break;
		}
		std::string dest_node = *(token_iter++);
		if (token_iter == graph_tokens.end()) {
			break;
		}
		std::string dest_socket = *(token_iter++);

		if (nodes_by_name.count(source_node) == 0 || nodes_by_name.count(dest_node) == 0) {
			continue;
		}

		NodeSocket* source = nodes_by_name[source_node]->get_socket_by_display_name(SocketInOut::Output, source_socket);
		NodeSocket* dest = nodes_by_name[dest_node]->get_socket_by_display_name(SocketInOut::Input, dest_socket);

		if (source == nullptr && dest == nullptr) {
			continue;
		}

		NodeConnection connection(source, dest);
		connections.push_back(connection);
	}

	// Mark all nodes as unchanged so an undo push isn't triggered
	for (EditorNode* node : nodes) {
		node->changed = false;
	}
}