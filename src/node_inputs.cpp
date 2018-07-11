#include "node_inputs.h"

#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::LightPathNode::LightPathNode(FloatPos position)
{
	world_pos = position;

	title = "Light Path";

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

	type = CyclesNodeType::LightPath;
}

CyclesShaderEditor::FresnelNode::FresnelNode(FloatPos position)
{
	world_pos = position;

	title = "Fresnel";

	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(fac_output);

	const auto ior_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "IOR", "IOR");
	ior_input->value = std::make_shared<FloatSocketValue>(1.45f, 0.0f, 1000.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(ior_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::Fresnel;
}

CyclesShaderEditor::LayerWeightNode::LayerWeightNode(FloatPos position)
{
	world_pos = position;

	title = "Layer Weight";

	const auto fresnel_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fresnel", "fresnel");
	const auto facing_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Facing", "facing");

	sockets.push_back(fresnel_output);
	sockets.push_back(facing_output);

	const auto blend_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Blend", "blend");
	blend_input->value = std::make_shared<FloatSocketValue>(0.5f, 0.0f, 1.0f);
	const auto normal_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::NORMAL, "Normal", "normal");

	sockets.push_back(blend_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::LayerWeight;
}

CyclesShaderEditor::CameraDataNode::CameraDataNode(FloatPos position)
{
	world_pos = position;

	title = "Camera Data";

	const auto vector_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "View Vector", "view_vector");
	const auto z_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "View Z Depth", "view_z_depth");
	const auto distance_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "View Distance", "view_distance");

	sockets.push_back(vector_output);
	sockets.push_back(z_depth_output);
	sockets.push_back(distance_output);

	type = CyclesNodeType::CameraData;
}

CyclesShaderEditor::TangentNode::TangentNode(FloatPos position)
{
	world_pos = position;

	title = "Tangent";

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

	type = CyclesNodeType::Tangent;
}

CyclesShaderEditor::TextureCoordinateNode::TextureCoordinateNode(FloatPos position)
{
	world_pos = position;

	title = "Texture Coordinate";

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

	type = CyclesNodeType::TextureCoordinate;
}

CyclesShaderEditor::GeometryNode::GeometryNode(FloatPos position)
{
	world_pos = position;

	title = "Geometry";

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

	type = CyclesNodeType::Geometry;
}

CyclesShaderEditor::ObjectInfoNode::ObjectInfoNode(FloatPos position)
{
	world_pos = position;

	title = "Object Info";

	const auto location_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::VECTOR, "Location", "location");
	const auto object_index_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Object Index", "object_index");
	const auto material_index_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Material Index", "material_index");
	const auto random_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Random", "random");

	sockets.push_back(location_output);
	sockets.push_back(object_index_output);
	sockets.push_back(material_index_output);
	sockets.push_back(random_output);

	type = CyclesNodeType::ObjectInfo;
}

CyclesShaderEditor::RGBNode::RGBNode(FloatPos position)
{
	world_pos = position;

	title = "RGB";

	const auto color_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::COLOR, "Color", "color");

	sockets.push_back(color_output);

	const auto value_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::COLOR, "Value", "value");
	value_input->value = std::make_shared<ColorSocketValue>(1.0f, 1.0f, 1.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::RGB;
}

CyclesShaderEditor::ValueNode::ValueNode(FloatPos position)
{
	world_pos = position;

	title = "Value";

	const auto value_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Value", "value");

	sockets.push_back(value_output);

	const auto value_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Value", "value");
	value_input->value = std::make_shared<FloatSocketValue>(0.0f, -10000.0f, 10000.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::Value;
}

CyclesShaderEditor::WireframeNode::WireframeNode(FloatPos position)
{
	world_pos = position;

	title = "Wireframe";

	const auto fac_output = std::make_shared<NodeSocket>(this, SocketIOType::OUTPUT, SocketType::FLOAT, "Fac", "fac");

	sockets.push_back(fac_output);

	const auto pixel_size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::BOOLEAN, "Use Pixel Size", "use_pixel_size");
	pixel_size_input->value = std::make_shared<BoolSocketValue>(false);
	const auto size_input = std::make_shared<NodeSocket>(this, SocketIOType::INPUT, SocketType::FLOAT, "Size", "size");
	size_input->value = std::make_shared<FloatSocketValue>(0.04f, 0.0f, 10000.0f);

	sockets.push_back(pixel_size_input);
	sockets.push_back(size_input);

	type = CyclesNodeType::Wireframe;
}
