#include "node_inputs.h"

#include <string>
#include <vector>

#include "float_pos.h"
#include "output.h"
#include "sockets.h"

CyclesShaderEditor::LightPathNode::LightPathNode(FloatPos position)
{
	world_pos = position;

	title = "Light Path";

	const std::shared_ptr<NodeSocket> camera_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Camera Ray", "is_camera_ray");
	const std::shared_ptr<NodeSocket> shadow_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Shadow Ray", "is_shadow_ray");
	const std::shared_ptr<NodeSocket> diffuse_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Diffuse Ray", "is_diffuse_ray");
	const std::shared_ptr<NodeSocket> glossy_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Glossy Ray", "is_glossy_ray");
	const std::shared_ptr<NodeSocket> singular_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Singular Ray", "is_singular_ray");
	const std::shared_ptr<NodeSocket> reflection_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Reflection Ray", "is_reflection_ray");
	const std::shared_ptr<NodeSocket> transmission_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Transmission Ray", "is_transmission_ray");
	const std::shared_ptr<NodeSocket> vol_scatter_ray_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Is Volume Scatter Ray", "is_volume_scatter_ray");
	const std::shared_ptr<NodeSocket> ray_length_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Ray Length", "ray_length");
	const std::shared_ptr<NodeSocket> ray_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Ray Depth", "ray_depth");
	const std::shared_ptr<NodeSocket> transparent_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Transparent Depth", "transparent_depth");
	const std::shared_ptr<NodeSocket> transmission_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Transmission Depth", "transmission_depth");

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

	const std::shared_ptr<NodeSocket> fac_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(fac_output);

	const std::shared_ptr<NodeSocket> ior_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "IOR", "IOR");
	ior_input->value = new FloatSocketValue(1.45f, 0.0f, 1000.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(ior_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::Fresnel;
}

CyclesShaderEditor::LayerWeightNode::LayerWeightNode(FloatPos position)
{
	world_pos = position;

	title = "Layer Weight";

	const std::shared_ptr<NodeSocket> fresnel_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Fresnel", "fresnel");
	const std::shared_ptr<NodeSocket> facing_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Facing", "facing");

	sockets.push_back(fresnel_output);
	sockets.push_back(facing_output);

	const std::shared_ptr<NodeSocket> blend_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Blend", "blend");
	blend_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	const std::shared_ptr<NodeSocket> normal_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(blend_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::LayerWeight;
}

CyclesShaderEditor::CameraDataNode::CameraDataNode(FloatPos position)
{
	world_pos = position;

	title = "Camera Data";

	const std::shared_ptr<NodeSocket> vector_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "View Vector", "view_vector");
	const std::shared_ptr<NodeSocket> z_depth_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "View Z Depth", "view_z_depth");
	const std::shared_ptr<NodeSocket> distance_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "View Distance", "view_distance");

	sockets.push_back(vector_output);
	sockets.push_back(z_depth_output);
	sockets.push_back(distance_output);

	type = CyclesNodeType::CameraData;
}

CyclesShaderEditor::TangentNode::TangentNode(FloatPos position)
{
	world_pos = position;

	title = "Tangent";

	const std::shared_ptr<NodeSocket> tangent_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Tangent", "tangent");

	sockets.push_back(tangent_output);

	const std::shared_ptr<NodeSocket> direction_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Direction", "direction");
	StringEnumSocketValue* direction_value = new StringEnumSocketValue();
	direction_value->enum_values.push_back(StringEnumPair("Radial", "radial"));
	direction_value->enum_values.push_back(StringEnumPair("UV Map", "uv_map"));
	direction_value->set_from_internal_name("radial");
	direction_input->value = direction_value;
	const std::shared_ptr<NodeSocket> axis_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::StringEnum, "Radial Axis", "axis");
	StringEnumSocketValue* axis_value = new StringEnumSocketValue();
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

	const std::shared_ptr<NodeSocket> generated_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Generated", "generated");
	const std::shared_ptr<NodeSocket> normal_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Normal", "normal");
	const std::shared_ptr<NodeSocket> uv_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "UV", "UV");
	const std::shared_ptr<NodeSocket> object_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Object", "object");
	const std::shared_ptr<NodeSocket> camera_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Camera", "camera");
	const std::shared_ptr<NodeSocket> window_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Window", "window");
	const std::shared_ptr<NodeSocket> reflection_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Reflection", "reflection");

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

	const std::shared_ptr<NodeSocket> position_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Position", "position");
	const std::shared_ptr<NodeSocket> normal_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Normal, "Normal", "normal");
	const std::shared_ptr<NodeSocket> tangent_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Tangent", "tangent");
	const std::shared_ptr<NodeSocket> true_normal_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Normal, "True Normal", "true_normal");
	const std::shared_ptr<NodeSocket> incoming_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Incoming", "incoming");
	const std::shared_ptr<NodeSocket> parametric_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Parametric", "parametric");
	const std::shared_ptr<NodeSocket> backfacing_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Backfacing", "backfacing");
	const std::shared_ptr<NodeSocket> pointiness_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Pointiness", "pointiness");

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

	const std::shared_ptr<NodeSocket> location_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Vector, "Location", "location");
	const std::shared_ptr<NodeSocket> object_index_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Object Index", "object_index");
	const std::shared_ptr<NodeSocket> material_index_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Material Index", "material_index");
	const std::shared_ptr<NodeSocket> random_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Random", "random");

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

	const std::shared_ptr<NodeSocket> color_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	const std::shared_ptr<NodeSocket> value_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Color, "Value", "value");
	value_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::RGB;
}

CyclesShaderEditor::ValueNode::ValueNode(FloatPos position)
{
	world_pos = position;

	title = "Value";

	const std::shared_ptr<NodeSocket> value_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Value", "value");

	sockets.push_back(value_output);

	const std::shared_ptr<NodeSocket> value_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Value", "value");
	value_input->value = new FloatSocketValue(0.0f, -10000.0f, 10000.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::Value;
}

CyclesShaderEditor::WireframeNode::WireframeNode(FloatPos position)
{
	world_pos = position;

	title = "Wireframe";

	const std::shared_ptr<NodeSocket> fac_output = std::make_shared<NodeSocket>(this, SocketIOType::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(fac_output);

	const std::shared_ptr<NodeSocket> pixel_size_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Boolean, "Use Pixel Size", "use_pixel_size");
	pixel_size_input->value = new BoolSocketValue(false);
	const std::shared_ptr<NodeSocket> size_input = std::make_shared<NodeSocket>(this, SocketIOType::Input, SocketType::Float, "Size", "size");
	size_input->value = new FloatSocketValue(0.04f, 0.0f, 10000.0f);

	sockets.push_back(pixel_size_input);
	sockets.push_back(size_input);

	type = CyclesNodeType::Wireframe;
}