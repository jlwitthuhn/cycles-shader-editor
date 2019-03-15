#include "serialize.h"

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <utility>

#include "common_enums.h"
#include "float_pos.h"
#include "node_base.h"
#include "node_colors.h"
#include "node_converter.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_outputs.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"
#include "output.h"
#include "sockets.h"
#include "util_parse.h"

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
		// Map is already inialized
		return;
	}

	type_to_code[CyclesNodeType::AmbientOcclusion] = std::string("ambient_occlusion");
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

	type_to_code[CyclesNodeType::MaxTex] = std::string("max_tex");
	type_to_code[CyclesNodeType::BrickTex] = std::string("brick_tex");
	type_to_code[CyclesNodeType::CheckerTex] = std::string("checker_tex");
	type_to_code[CyclesNodeType::GradientTex] = std::string("gradient_tex");
	type_to_code[CyclesNodeType::MagicTex] = std::string("magic_tex");
	type_to_code[CyclesNodeType::MusgraveTex] = std::string("musgrave_tex");
	type_to_code[CyclesNodeType::NoiseTex] = std::string("noise_tex");
	type_to_code[CyclesNodeType::VoronoiTex] = std::string("voronoi_tex");
	type_to_code[CyclesNodeType::WaveTex] = std::string("wave_tex");

	type_to_code[CyclesNodeType::LightPath] = std::string("light_path");
	type_to_code[CyclesNodeType::Fresnel] = std::string("fresnel");
	type_to_code[CyclesNodeType::LayerWeight] = std::string("layer_weight");
	type_to_code[CyclesNodeType::CameraData] = std::string("camera_data");
	type_to_code[CyclesNodeType::Tangent] = std::string("tangent");
	type_to_code[CyclesNodeType::TextureCoordinate] = std::string("texture_coordinate");
	type_to_code[CyclesNodeType::Geometry] = std::string("geometry");
	type_to_code[CyclesNodeType::ObjectInfo] = std::string("object_info");
	type_to_code[CyclesNodeType::RGB] = std::string("rgb");
	type_to_code[CyclesNodeType::Value] = std::string("value");
	type_to_code[CyclesNodeType::Wireframe] = std::string("wireframe");

	type_to_code[CyclesNodeType::MixRGB] = std::string("mix_rgb");
	type_to_code[CyclesNodeType::Invert] = std::string("invert");
	type_to_code[CyclesNodeType::LightFalloff] = std::string("light_falloff");
	type_to_code[CyclesNodeType::HSV] = std::string("hsv");
	type_to_code[CyclesNodeType::Gamma] = std::string("gamma");
	type_to_code[CyclesNodeType::BrightnessContrast] = std::string("bright_contrast");
	type_to_code[CyclesNodeType::RGBCurves] = std::string("rgb_curves");

	type_to_code[CyclesNodeType::Bump] = std::string("bump");
	type_to_code[CyclesNodeType::NormalMap] = std::string("normal_map");
	type_to_code[CyclesNodeType::VectorTransform] = std::string("vector_transform");

	type_to_code[CyclesNodeType::Blackbody] = std::string("blackbody");
	type_to_code[CyclesNodeType::CombineHSV] = std::string("combine_hsv");
	type_to_code[CyclesNodeType::CombineRGB] = std::string("combine_rgb");
	type_to_code[CyclesNodeType::CombineXYZ] = std::string("combine_xyz");
	type_to_code[CyclesNodeType::Math] = std::string("math");
	type_to_code[CyclesNodeType::RGBtoBW] = std::string("rgb_to_bw");
	type_to_code[CyclesNodeType::SeparateHSV] = std::string("separate_hsv");
	type_to_code[CyclesNodeType::SeparateRGB] = std::string("separate_rgb");
	type_to_code[CyclesNodeType::SeparateXYZ] = std::string("separate_xyz");
	type_to_code[CyclesNodeType::VectorMath] = std::string("vector_math");
	type_to_code[CyclesNodeType::Wavelength] = std::string("wavelength");

	type_to_code[CyclesNodeType::MaterialOutput] = std::string("out_material");

	for (std::pair<CyclesNodeType, std::string> this_pair : type_to_code) {
		code_to_type[this_pair.second] = this_pair.first;
	}

	assert(code_to_type.size() == type_to_code.size());
}

static std::list<std::string> tokenize_string(const std::string& input, const char delim)
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

static std::string serialize_curve(const CyclesShaderEditor::OutputCurve& curve)
{
	constexpr char CURVE_SEPARATOR = ',';
	std::stringstream curve_stream;
	curve_stream << "curve00" << CURVE_SEPARATOR;
	if (curve.enum_curve_interp == static_cast<int>(CyclesShaderEditor::CurveInterpolation::CUBIC_HERMITE)) {
		curve_stream << "cubic_hermite" << CURVE_SEPARATOR;
	}
	else {
		curve_stream << "linear" << CURVE_SEPARATOR;
	}
	curve_stream << curve.control_points.size();

	curve_stream.precision(6);
	for (const CyclesShaderEditor::Float2& this_point : curve.control_points) {
		curve_stream << CURVE_SEPARATOR << this_point.x << CURVE_SEPARATOR << this_point.y ;
	}

	return curve_stream.str();
}

static void deserialize_curve(std::string serialized_curve, std::shared_ptr<CyclesShaderEditor::CurveSocketValue> curve_value)
{
	curve_value->reset_value();
	constexpr char CURVE_SEPARATOR = ',';
	std::list<std::string> tokenized_input = tokenize_string(serialized_curve, CURVE_SEPARATOR);

	// The input must have at least 5 entries to be valid
	if (tokenized_input.size() < 5) {
		return;
	}

	std::list<std::string>::iterator input_iter = tokenized_input.begin();
	std::string identifier = *(input_iter++);
	std::string interpolation_str = *(input_iter++);
	std::string control_point_count_str = *(input_iter++);

	// Make sure we understand this curve format
	if (identifier != "curve00") {
		return;
	}

	// Make sure the number of points and total number of tokens match
	const std::size_t control_point_count = static_cast<std::size_t>(std::stoul(control_point_count_str));
	if (control_point_count < 1 || tokenized_input.size() != 3 + 2 * control_point_count) {
		return;
	}

	curve_value->curve_points.clear();
	for (std::size_t points_copied = 0; points_copied < control_point_count; points_copied++) {
		const float x = CyclesShaderEditor::locale_safe_stof(*(input_iter++));
		const float y = CyclesShaderEditor::locale_safe_stof(*(input_iter++));
		CyclesShaderEditor::FloatPos this_point(x, y);
		curve_value->curve_points.push_back(this_point);
	}
	curve_value->sort_curve_points();

	if (interpolation_str == "cubic_hermite") {
		curve_value->curve_interp = CyclesShaderEditor::CurveInterpolation::CUBIC_HERMITE;
	}
	else {
		curve_value->curve_interp = CyclesShaderEditor::CurveInterpolation::LINEAR;
	}
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
	for (std::pair<std::string, OutputCurve> this_pair : node.curve_values) {
		node_stream << this_pair.first << SEPARATOR << serialize_curve(this_pair.second) << SEPARATOR;
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

void CyclesShaderEditor::generate_output_lists(
	const std::list<std::shared_ptr<EditorNode>>& node_list,
	const std::list<NodeConnection>& connection_list,
	std::vector<OutputNode>& out_node_list,
	std::vector<OutputConnection>& out_connection_list)
{
	using namespace CyclesShaderEditor;

	std::map<EditorNode*, std::string> node_to_name_map;

	for (const auto this_node : node_list) {
		OutputNode this_out_node;
		this_out_node.name = create_node_name();

		this_node->update_output_node(this_out_node);

		out_node_list.push_back(this_out_node);
		node_to_name_map[this_node.get()] = this_out_node.name;
	}

	for (NodeConnection this_connection : connection_list) {
		auto begin_ptr = this_connection.begin_socket.lock();
		auto end_ptr = this_connection.end_socket.lock();
		if (begin_ptr && end_ptr) {
			OutputConnection this_out_connection;
			this_out_connection.source_node = node_to_name_map[begin_ptr->parent];
			this_out_connection.dest_node = node_to_name_map[end_ptr->parent];
			this_out_connection.source_socket = begin_ptr->display_name;
			this_out_connection.dest_socket = end_ptr->display_name;
			out_connection_list.push_back(this_out_connection);
		}
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

static std::shared_ptr<CyclesShaderEditor::EditorNode> create_node_from_type(CyclesShaderEditor::CyclesNodeType type) {
	using namespace CyclesShaderEditor;
	const FloatPos pos(0.0f, 0.0f);
	switch (type) {
		case CyclesNodeType::AmbientOcclusion:
		{
			return std::make_shared<AmbientOcculsionNode>(pos);
		}
		case CyclesNodeType::PrincipledBSDF:
		{
			return std::make_shared<PrincipledBSDFNode>(pos);
		}
		case CyclesNodeType::MixShader:
		{
			return std::make_shared<MixShaderNode>(pos);
		}
		case CyclesNodeType::AddShader:
		{
			return std::make_shared<AddShaderNode>(pos);
		}
		case CyclesNodeType::DiffuseBSDF:
		{
			return std::make_shared<DiffuseBSDFNode>(pos);
		}
		case CyclesNodeType::GlossyBSDF:
		{
			return std::make_shared<GlossyBSDFNode>(pos);
		}
		case CyclesNodeType::TransparentBSDF:
		{
			return std::make_shared<TransparentBSDFNode>(pos);
		}
		case CyclesNodeType::RefractionBSDF:
		{
			return std::make_shared<RefractionBSDFNode>(pos);
		}
		case CyclesNodeType::GlassBSDF:
		{
			return std::make_shared<GlassBSDFNode>(pos);
		}
		case CyclesNodeType::TranslucentBSDF:
		{
			return std::make_shared<TranslucentBSDFNode>(pos);
		}
		case CyclesNodeType::AnisotropicBSDF:
		{
			return std::make_shared<AnisotropicBSDFNode>(pos);
		}
		case CyclesNodeType::VelvetBSDF:
		{
			return std::make_shared<VelvetBSDFNode>(pos);
		}
		case CyclesNodeType::ToonBSDF:
		{
			return std::make_shared<ToonBSDFNode>(pos);
		}
		case CyclesNodeType::SubsurfaceScattering:
		{
			return std::make_shared<SubsurfaceScatteringNode>(pos);
		}
		case CyclesNodeType::Emission:
		{
			return std::make_shared<EmissionNode>(pos);
		}
		case CyclesNodeType::HairBSDF:
		{
			return std::make_shared<HairBSDFNode>(pos);
		}
		case CyclesNodeType::Holdout:
		{
			return std::make_shared<HoldoutNode>(pos);
		}
		case CyclesNodeType::VolAbsorption:
		{
			return std::make_shared<VolumeAbsorptionNode>(pos);
		}
		case CyclesNodeType::VolScatter:
		{
			return std::make_shared<VolumeScatterNode>(pos);
		}
		case CyclesNodeType::BrickTex:
		{
			return std::make_shared<BrickTextureNode>(pos);
		}
		case CyclesNodeType::NoiseTex:
		{
			return std::make_shared<NoiseTextureNode>(pos);
		}
		case CyclesNodeType::WaveTex:
		{
			return std::make_shared<WaveTextureNode>(pos);
		}
		case CyclesNodeType::VoronoiTex:
		{
			return std::make_shared<VoronoiTextureNode>(pos);
		}
		case CyclesNodeType::MusgraveTex:
		{
			return std::make_shared<MusgraveTextureNode>(pos);
		}
		case CyclesNodeType::GradientTex:
		{
			return std::make_shared<GradientTextureNode>(pos);
		}
		case CyclesNodeType::MagicTex:
		{
			return std::make_shared<MagicTextureNode>(pos);
		}
		case CyclesNodeType::CheckerTex:
		{
			return std::make_shared<CheckerTextureNode>(pos);
		}
		case CyclesNodeType::MaxTex:
		{
			return std::make_shared<MaxTexmapShaderNode>(pos);
		}
		case CyclesNodeType::LightPath:
		{
			return std::make_shared<LightPathNode>(pos);
		}
		case CyclesNodeType::Fresnel:
		{
			return std::make_shared<FresnelNode>(pos);
		}
		case CyclesNodeType::LayerWeight:
		{
			return std::make_shared<LayerWeightNode>(pos);
		}
		case CyclesNodeType::CameraData:
		{
			return std::make_shared<CameraDataNode>(pos);
		}
		case CyclesNodeType::Tangent:
		{
			return std::make_shared<TangentNode>(pos);
		}
		case CyclesNodeType::TextureCoordinate:
		{
			return std::make_shared<TextureCoordinateNode>(pos);
		}
		case CyclesNodeType::Geometry:
		{
			return std::make_shared<GeometryNode>(pos);
		}
		case CyclesNodeType::ObjectInfo:
		{
			return std::make_shared<ObjectInfoNode>(pos);
		}
		case CyclesNodeType::RGB:
		{
			return std::make_shared<RGBNode>(pos);
		}
		case CyclesNodeType::Wireframe:
		{
			return std::make_shared<WireframeNode>(pos);
		}
		case CyclesNodeType::Value:
		{
			return std::make_shared<ValueNode>(pos);
		}
		case CyclesNodeType::MixRGB:
		{
			return std::make_shared<MixRGBNode>(pos);
		}
		case CyclesNodeType::Invert:
		{
			return std::make_shared<InvertNode>(pos);
		}
		case CyclesNodeType::LightFalloff:
		{
			return std::make_shared<LightFalloffNode>(pos);
		}
		case CyclesNodeType::HSV:
		{
			return std::make_shared<HSVNode>(pos);
		}
		case CyclesNodeType::Gamma:
		{
			return std::make_shared<GammaNode>(pos);
		}
		case CyclesNodeType::BrightnessContrast:
		{
			return std::make_shared<BrightnessContrastNode>(pos);
		}
		case CyclesNodeType::RGBCurves:
		{
			return std::make_shared<RGBCurvesNode>(pos);
		}
		case CyclesNodeType::Bump:
		{
			return std::make_shared<BumpNode>(pos);
		}
		case CyclesNodeType::NormalMap:
		{
			return std::make_shared<NormalMapNode>(pos);
		}
		case CyclesNodeType::VectorTransform:
		{
			return std::make_shared<VectorTransformNode>(pos);
		}
		case CyclesNodeType::Blackbody:
		{
			return std::make_shared<BlackbodyNode>(pos);
		}
		case CyclesNodeType::CombineHSV:
		{
			return std::make_shared<CombineHSVNode>(pos);
		}
		case CyclesNodeType::CombineRGB:
		{
			return std::make_shared<CombineRGBNode>(pos);
		}
		case CyclesNodeType::CombineXYZ:
		{
			return std::make_shared<CombineXYZNode>(pos);
		}
		case CyclesNodeType::Math:
		{
			return std::make_shared<MathNode>(pos);
		}
		case CyclesNodeType::RGBtoBW:
		{
			return std::make_shared<RGBToBWNode>(pos);
		}
		case CyclesNodeType::SeparateHSV:
		{
			return std::make_shared<SeparateHSVNode>(pos);
		}
		case CyclesNodeType::SeparateRGB:
		{
			return std::make_shared<SeparateRGBNode>(pos);
		}
		case CyclesNodeType::SeparateXYZ:
		{
			return std::make_shared<SeparateXYZNode>(pos);
		}
		case CyclesNodeType::VectorMath:
		{
			return std::make_shared<VectorMathNode>(pos);
		}
		case CyclesNodeType::Wavelength:
		{
			return std::make_shared<WavelengthNode>(pos);
		}
		case CyclesNodeType::MaterialOutput:
		{
			return std::make_shared<MaterialOutputNode>(pos);
		}
		case CyclesNodeType::Unknown:
		case CyclesNodeType::Count:
		{
			return nullptr;
		}
	}

	return nullptr;
}

static std::shared_ptr<CyclesShaderEditor::EditorNode> deserialize_node(std::list<std::string>& tokens, std::map<std::string, CyclesShaderEditor::EditorNode*>& nodes_by_name)
{
	using namespace CyclesShaderEditor;

	initialize_maps();

	if (tokens.size() < 4) {
		return nullptr;
	}

	std::list<std::string>::iterator token_iter = tokens.begin();

	std::string type_code = *(token_iter++);
	std::string name = *(token_iter++);
	float x_position = CyclesShaderEditor::locale_safe_stof(*(token_iter++));
	float y_position = CyclesShaderEditor::locale_safe_stof(*(token_iter++));

	std::map<std::string, std::string> params;
	while (token_iter != tokens.end()) {
		std::string param_name = *(token_iter++);
		if (token_iter == tokens.end()) {
			break;
		}
		std::string value = *(token_iter++);
		params[param_name] = value;
	}

	if (code_to_type.count(type_code) == 0) {
		// Unknown type
		return nullptr;
	}

	CyclesNodeType type = code_to_type[type_code];

	std::shared_ptr<EditorNode> result = create_node_from_type(type);
	result->world_pos = FloatPos(x_position, y_position);

	if (result == nullptr) {
		return nullptr;
	}

	for (std::pair<std::string, std::string> this_param : params) {
		std::weak_ptr<NodeSocket> this_socket = result->get_socket_by_internal_name(SocketIOType::INPUT, this_param.first);

		if (auto this_socket_ptr = this_socket.lock()) {
			switch (this_socket_ptr->socket_type) {

			case SocketType::FLOAT:
				this_socket_ptr->set_float_val(CyclesShaderEditor::locale_safe_stof(this_param.second));
				break;

			case SocketType::COLOR:
			case SocketType::VECTOR:
			{
				std::list<std::string> floats = tokenize_string(this_param.second, ',');
				if (floats.size() != 3) {
					break;
				}
				std::list<std::string>::iterator float_iter = floats.begin();
				std::string x = *(float_iter++);
				std::string y = *(float_iter++);
				std::string z = *(float_iter++);
				this_socket_ptr->set_float3_val(CyclesShaderEditor::locale_safe_stof(x), CyclesShaderEditor::locale_safe_stof(y), CyclesShaderEditor::locale_safe_stof(z));
				break;
			}

			case SocketType::STRING_ENUM:
			{
				const auto string_val = std::dynamic_pointer_cast<StringEnumSocketValue>(this_socket_ptr->value);
				if (string_val) {
					string_val->set_from_internal_name(this_param.second);
				}
				break;
			}

			case SocketType::INT:
			{
				const auto int_val = std::dynamic_pointer_cast<IntSocketValue>(this_socket_ptr->value);
				if (int_val) {
					int_val->set_value(std::stoi(this_param.second));
				}
				break;
			}

			case SocketType::BOOLEAN:
			{
				const auto bool_val = std::dynamic_pointer_cast<BoolSocketValue>(this_socket_ptr->value);
				if (bool_val) {
					bool_val->value = std::stoi(this_param.second) != 0;
				}
				break;
			}

			case SocketType::CURVE:
			{
				const auto curve_val = std::dynamic_pointer_cast<CurveSocketValue>(this_socket_ptr->value);
				if (curve_val) {
					deserialize_curve(this_param.second, curve_val);
				}
			}
			default:
				break;
			}
		}
	}

	nodes_by_name[name] = result.get();

	return result;
}

void CyclesShaderEditor::deserialize_graph(
	const std::string& graph,
	std::list<std::shared_ptr<CyclesShaderEditor::EditorNode>>& nodes,
	std::list<NodeConnection>& connections )
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

		std::shared_ptr<EditorNode> node = deserialize_node(node_tokens, nodes_by_name);
		if (node) {
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

		const std::weak_ptr<NodeSocket> source = nodes_by_name[source_node]->get_socket_by_display_name(SocketIOType::OUTPUT, source_socket);
		const std::weak_ptr<NodeSocket> dest = nodes_by_name[dest_node]->get_socket_by_display_name(SocketIOType::INPUT, dest_socket);

		if (source.expired() || dest.expired()) {
			continue;
		}

		NodeConnection connection(source, dest);
		connections.push_back(connection);
	}

	// Mark all nodes as unchanged so an undo push isn't triggered
	for (const auto node : nodes) {
		node->changed = false;
	}
}
