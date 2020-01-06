#include "node_inputs.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "output.h"
#include "sockets.h"

cse::AmbientOcculsionNode::AmbientOcculsionNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::AmbientOcclusion, "Ambient Occlusion")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");
	const auto ao_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "AO", "ao");

	sockets.push_back(color_output);
	sockets.push_back(ao_output);

	const auto samples_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Samples", "samples");
	samples_input->value = std::make_shared<IntSocketValue>(16, 1, 128);
	const auto inside_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Inside", "inside");
	inside_input->value = std::make_shared<BoolSocketValue>(false);
	const auto only_local_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Only Local", "only_local");
	only_local_input->value = std::make_shared<BoolSocketValue>(false);
	const auto color_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Color", "color");
	color_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	const auto distance_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Distance", "distance");
	distance_input->value = std::make_shared<FloatSocketValue>(1.0f, 0.0f, 10000.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(samples_input);
	sockets.push_back(inside_input);
	sockets.push_back(only_local_input);
	sockets.push_back(color_input);
	sockets.push_back(distance_input);
	sockets.push_back(normal_input);
}

cse::BevelNode::BevelNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Bevel, "Bevel")
{
	world_pos = position;

	const auto normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(normal_output);

	const auto samples_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::INT, "Samples", "samples");
	samples_input->value = std::make_shared<IntSocketValue>(4, 2, 16);
	const auto radius_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Radius", "radius");
	radius_input->value = std::make_shared<FloatSocketValue>(0.05f, 0.0f, 5000.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(samples_input);
	sockets.push_back(radius_input);
	sockets.push_back(normal_input);
}

cse::CameraDataNode::CameraDataNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::CameraData, "Camera Data")
{
	world_pos = position;

	const auto vector_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "View Vector", "view_vector");
	const auto z_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "View Z Depth", "view_z_depth");
	const auto distance_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "View Distance", "view_distance");

	sockets.push_back(vector_output);
	sockets.push_back(z_depth_output);
	sockets.push_back(distance_output);
}

cse::FresnelNode::FresnelNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Fresnel, "Fresnel")
{
	world_pos = position;

	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(fac_output);

	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "IOR");
	ior_input->value = std::make_shared<FloatSocketValue>(1.45f, 0.0f, 1000.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(ior_input);
	sockets.push_back(normal_input);
}

cse::GeometryNode::GeometryNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Geometry, "Geometry")
{
	world_pos = position;

	const auto position_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Position", "position");
	const auto normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::NORMAL, "Normal", "normal");
	const auto tangent_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Tangent", "tangent");
	const auto true_normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::NORMAL, "True Normal", "true_normal");
	const auto incoming_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Incoming", "incoming");
	const auto parametric_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Parametric", "parametric");
	const auto backfacing_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Backfacing", "backfacing");
	const auto pointiness_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Pointiness", "pointiness");

	sockets.push_back(position_output);
	sockets.push_back(normal_output);
	sockets.push_back(tangent_output);
	sockets.push_back(true_normal_output);
	sockets.push_back(incoming_output);
	sockets.push_back(parametric_output);
	sockets.push_back(backfacing_output);
	sockets.push_back(pointiness_output);
}

cse::LayerWeightNode::LayerWeightNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::LayerWeight, "Layer Weight")
{
	world_pos = position;

	const auto fresnel_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fresnel", "fresnel");
	const auto facing_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Facing", "facing");

	sockets.push_back(fresnel_output);
	sockets.push_back(facing_output);

	const auto blend_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Blend", "blend");
	blend_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(blend_input);
	sockets.push_back(normal_input);
}

cse::LightPathNode::LightPathNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::LightPath, "Light Path")
{
	world_pos = position;

	const auto camera_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Camera Ray", "is_camera_ray");
	const auto shadow_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Shadow Ray", "is_shadow_ray");
	const auto diffuse_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Diffuse Ray", "is_diffuse_ray");
	const auto glossy_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Glossy Ray", "is_glossy_ray");
	const auto singular_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Singular Ray", "is_singular_ray");
	const auto reflection_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Reflection Ray", "is_reflection_ray");
	const auto transmission_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Transmission Ray", "is_transmission_ray");
	const auto vol_scatter_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Is Volume Scatter Ray", "is_volume_scatter_ray");
	const auto ray_length_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Ray Length", "ray_length");
	const auto ray_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Ray Depth", "ray_depth");
	const auto transparent_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Transparent Depth", "transparent_depth");
	const auto transmission_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Transmission Depth", "transmission_depth");

	sockets.push_back(camera_ray_output);
	sockets.push_back(shadow_ray_output);
	sockets.push_back(diffuse_ray_output);
	sockets.push_back(glossy_ray_output);
	sockets.push_back(singular_ray_output);
	sockets.push_back(reflection_ray_output);
	sockets.push_back(transmission_ray_output);
	sockets.push_back(vol_scatter_ray_output);
	sockets.push_back(ray_length_output);
	sockets.push_back(ray_depth_output);
	sockets.push_back(transparent_depth_output);
	sockets.push_back(transmission_depth_output);
}

cse::ObjectInfoNode::ObjectInfoNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::ObjectInfo, "Object Info")
{
	world_pos = position;

	const auto location_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Location", "location");
	const auto object_index_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Object Index", "object_index");
	const auto material_index_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Material Index", "material_index");
	const auto random_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Random", "random");

	sockets.push_back(location_output);
	sockets.push_back(object_index_output);
	sockets.push_back(material_index_output);
	sockets.push_back(random_output);
}

cse::RGBNode::RGBNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::RGB, "RGB")
{
	world_pos = position;

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto value_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Value", "value");
	value_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);
}

cse::TangentNode::TangentNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Tangent, "Tangent")
{
	world_pos = position;

	const auto tangent_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Tangent", "tangent");

	sockets.push_back(tangent_output);

	const auto direction_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Direction", "direction");
	const auto direction_value = std::make_shared<StringEnumSocketValue>();
	direction_value->enum_values.push_back(StringEnumPair("Radial", "radial"));
	direction_value->enum_values.push_back(StringEnumPair("UV Map", "uv_map"));
	direction_value->set_from_internal_name("radial");
	direction_input->value = direction_value;
	const auto axis_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::STRING_ENUM, "Radial Axis", "axis");
	const auto axis_value = std::make_shared<StringEnumSocketValue>();
	axis_value->enum_values.push_back(StringEnumPair("X", "x"));
	axis_value->enum_values.push_back(StringEnumPair("Y", "y"));
	axis_value->enum_values.push_back(StringEnumPair("Z", "z"));
	axis_value->set_from_internal_name("z");
	axis_input->value = axis_value;

	sockets.push_back(direction_input);
	sockets.push_back(axis_input);
}

cse::TextureCoordinateNode::TextureCoordinateNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::TextureCoordinate, "Texture Coordinate")
{
	world_pos = position;

	const auto generated_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Generated", "generated");
	const auto normal_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Normal", "normal");
	const auto uv_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "UV", "UV");
	const auto object_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Object", "object");
	const auto camera_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Camera", "camera");
	const auto window_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Window", "window");
	const auto reflection_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Reflection", "reflection");

	sockets.push_back(generated_output);
	sockets.push_back(normal_output);
	sockets.push_back(uv_output);
	sockets.push_back(object_output);
	sockets.push_back(camera_output);
	sockets.push_back(window_output);
	sockets.push_back(reflection_output);
}

cse::ValueNode::ValueNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Value, "Value")
{
	world_pos = position;

	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(value_output);

	const auto value_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value", "value");
	value_input->value = std::make_shared<FloatSocketValue>(0.0f, -10000.0f, 10000.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);
}

cse::WireframeNode::WireframeNode(const Float2 position) : EditableNode(NodeCategory::INPUT, CyclesNodeType::Wireframe, "Wireframe")
{
	world_pos = position;

	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(fac_output);

	const auto pixel_size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Use Pixel Size", "use_pixel_size");
	pixel_size_input->value = std::make_shared<BoolSocketValue>(false);
	const auto size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Size", "size");
	size_input->value = std::make_shared<FloatSocketValue>(0.04f, 0.0f, 10000.0f);

	sockets.push_back(pixel_size_input);
	sockets.push_back(size_input);
}
