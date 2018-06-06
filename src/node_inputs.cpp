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

	NodeSocket* camera_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Camera Ray", "is_camera_ray");
	NodeSocket* shadow_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Shadow Ray", "is_shadow_ray");
	NodeSocket* diffuse_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Diffuse Ray", "is_diffuse_ray");
	NodeSocket* glossy_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Glossy Ray", "is_glossy_ray");
	NodeSocket* singular_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Singular Ray", "is_singular_ray");
	NodeSocket* reflection_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Reflection Ray", "is_reflection_ray");
	NodeSocket* transmission_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Transmission Ray", "is_transmission_ray");
	NodeSocket* vol_scatter_ray_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Is Volume Scatter Ray", "is_volume_scatter_ray");
	NodeSocket* ray_length_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Ray Length", "ray_length");
	NodeSocket* ray_depth_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Ray Depth", "ray_depth");
	NodeSocket* transparent_depth_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Transparent Depth", "transparent_depth");
	NodeSocket* transmission_depth_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Transmission Depth", "transmission_depth");

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

	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(fac_output);

	NodeSocket* ior_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "IOR", "IOR");
	ior_input->value = new FloatSocketValue(1.45f, 0.0f, 1000.0f);
	NodeSocket* normal_input = new NodeSocket(this, SocketInOut::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(ior_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::Fresnel;
}

CyclesShaderEditor::LayerWeightNode::LayerWeightNode(FloatPos position)
{
	world_pos = position;

	title = "Layer Weight";

	NodeSocket* fresnel_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fresnel", "fresnel");
	NodeSocket* facing_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Facing", "facing");

	sockets.push_back(fresnel_output);
	sockets.push_back(facing_output);

	NodeSocket* blend_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Blend", "blend");
	blend_input->value = new FloatSocketValue(0.5f, 0.0f, 1.0f);
	NodeSocket* normal_input = new NodeSocket(this, SocketInOut::Input, SocketType::Normal, "Normal", "normal");

	sockets.push_back(blend_input);
	sockets.push_back(normal_input);

	type = CyclesNodeType::LayerWeight;
}

CyclesShaderEditor::CameraDataNode::CameraDataNode(FloatPos position)
{
	world_pos = position;

	title = "Camera Data";

	NodeSocket* vector_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "View Vector", "view_vector");
	NodeSocket* z_depth_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "View Z Depth", "view_z_depth");
	NodeSocket* distance_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "View Distance", "view_distance");

	sockets.push_back(vector_output);
	sockets.push_back(z_depth_output);
	sockets.push_back(distance_output);

	type = CyclesNodeType::CameraData;
}

CyclesShaderEditor::TangentNode::TangentNode(FloatPos position)
{
	world_pos = position;

	title = "Tangent";

	NodeSocket* tangent_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Tangent", "tangent");

	sockets.push_back(tangent_output);

	NodeSocket* direction_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Direction", "direction");
	StringEnumSocketValue* direction_value = new StringEnumSocketValue();
	direction_value->enum_values.push_back(StringEnumPair("Radial", "radial"));
	direction_value->enum_values.push_back(StringEnumPair("UV Map", "uv_map"));
	direction_value->set_from_internal_name("radial");
	direction_input->value = direction_value;
	NodeSocket* axis_input = new NodeSocket(this, SocketInOut::Input, SocketType::StringEnum, "Radial Axis", "axis");
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

	NodeSocket* generated_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Generated", "generated");
	NodeSocket* normal_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Normal", "normal");
	NodeSocket* uv_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "UV", "UV");
	NodeSocket* object_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Object", "object");
	NodeSocket* camera_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Camera", "camera");
	NodeSocket* window_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Window", "window");
	NodeSocket* reflection_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Reflection", "reflection");

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

	NodeSocket* position_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Position", "position");
	NodeSocket* normal_output = new NodeSocket(this, SocketInOut::Output, SocketType::Normal, "Normal", "normal");
	NodeSocket* tangent_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Tangent", "tangent");
	NodeSocket* true_normal_output = new NodeSocket(this, SocketInOut::Output, SocketType::Normal, "True Normal", "true_normal");
	NodeSocket* incoming_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Incoming", "incoming");
	NodeSocket* parametric_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Parametric", "parametric");
	NodeSocket* backfacing_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Backfacing", "backfacing");
	NodeSocket* pointiness_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Pointiness", "pointiness");

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

	NodeSocket* location_output = new NodeSocket(this, SocketInOut::Output, SocketType::Vector, "Location", "location");
	NodeSocket* object_index_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Object Index", "object_index");
	NodeSocket* material_index_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Material Index", "material_index");
	NodeSocket* random_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Random", "random");

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

	NodeSocket* color_output = new NodeSocket(this, SocketInOut::Output, SocketType::Color, "Color", "color");

	sockets.push_back(color_output);

	NodeSocket* value_input = new NodeSocket(this, SocketInOut::Input, SocketType::Color, "Value", "value");
	value_input->value = new ColorSocketValue(1.0f, 1.0f, 1.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::RGB;
}

CyclesShaderEditor::ValueNode::ValueNode(FloatPos position)
{
	world_pos = position;

	title = "Value";

	NodeSocket* value_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Value", "value");

	sockets.push_back(value_output);

	NodeSocket* value_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Value", "value");
	value_input->value = new FloatSocketValue(0.0f, -10000.0f, 10000.0f);
	value_input->draw_socket = false;

	sockets.push_back(value_input);

	type = CyclesNodeType::Value;
}

CyclesShaderEditor::WireframeNode::WireframeNode(FloatPos position)
{
	world_pos = position;

	title = "Wireframe";

	NodeSocket* fac_output = new NodeSocket(this, SocketInOut::Output, SocketType::Float, "Fac", "fac");

	sockets.push_back(fac_output);

	NodeSocket* pixel_size_input = new NodeSocket(this, SocketInOut::Input, SocketType::Boolean, "Use Pixel Size", "use_pixel_size");
	pixel_size_input->value = new BoolSocketValue(false);
	NodeSocket* size_input = new NodeSocket(this, SocketInOut::Input, SocketType::Float, "Size", "size");
	size_input->value = new FloatSocketValue(0.04f, 0.0f, 10000.0f);

	sockets.push_back(pixel_size_input);
	sockets.push_back(size_input);

	type = CyclesNodeType::Wireframe;
}