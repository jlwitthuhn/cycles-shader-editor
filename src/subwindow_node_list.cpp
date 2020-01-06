#include "subwindow_node_list.h"

#include <string>
#include <utility>

#include <nanovg.h>
#include <GLFW/glfw3.h>

#include "config.h"

#include "button_category.h"
#include "buttons_nodes.h"
#include "gui_sizes.h"
#include "node_colors.h"
#include "node_converter.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"

cse::NodeCreationHelper::NodeCreationHelper()
{

}

bool cse::NodeCreationHelper::is_ready() const
{
	return current_node.use_count() > 0;
}

void cse::NodeCreationHelper::set_node(const std::shared_ptr<EditableNode>& new_node)
{
	current_node = new_node;
}

void cse::NodeCreationHelper::clear()
{
	current_node = std::shared_ptr<EditableNode>();
}

std::shared_ptr<cse::EditableNode> cse::NodeCreationHelper::take()
{
	if (current_node) {
		return std::move(current_node);
	}
	return std::unique_ptr<EditableNode>();
}

cse::NodeListSubwindow::NodeListSubwindow(const std::weak_ptr<NodeCreationHelper> node_creation_helper, const Float2 screen_position) :
	NodeEditorSubwindow(screen_position, "Create Node"),
	node_creation_helper(node_creation_helper)
{
	subwindow_width = UI_SUBWIN_NODE_LIST_WIDTH;

	// Category buttons
	auto cat_input_button = std::make_unique<NodeCategoryButton>("Input");
	auto cat_shader_button = std::make_unique<NodeCategoryButton>("Shader");
	auto cat_texture_button = std::make_unique<NodeCategoryButton>("Texture");
	auto cat_color_button = std::make_unique<NodeCategoryButton>("Color");
	auto cat_vector_button = std::make_unique<NodeCategoryButton>("Vector");
	auto cat_converter_button = std::make_unique<NodeCategoryButton>("Converter");

	cat_shader_button->selected = true;

	// Input buttons
	{
		auto ao_button = std::make_unique<GenericNodeButton<AmbientOcculsionNode>>();
		auto bevel_button = std::make_unique<GenericNodeButton<BevelNode>>();
		auto camera_data_button = std::make_unique<GenericNodeButton<CameraDataNode>>();
		auto fresnel_button = std::make_unique<GenericNodeButton<FresnelNode>>();
		auto geometry_button = std::make_unique<GenericNodeButton<GeometryNode>>();
		auto layer_weight_button = std::make_unique<GenericNodeButton<LayerWeightNode>>();
		auto light_path_button = std::make_unique<GenericNodeButton<LightPathNode>>();
		auto object_info_button = std::make_unique<GenericNodeButton<ObjectInfoNode>>();
		auto rgb_button = std::make_unique<GenericNodeButton<RGBNode>>();
		auto tangent_button = std::make_unique<GenericNodeButton<TangentNode>>();
		auto tex_coord_button = std::make_unique<GenericNodeButton<TextureCoordinateNode>>();
		auto value_button = std::make_unique<GenericNodeButton<ValueNode>>();
		auto wireframe_button = std::make_unique<GenericNodeButton<WireframeNode>>();

		cat_input_button->node_buttons.push_back(std::move(ao_button));
		cat_input_button->node_buttons.push_back(std::move(bevel_button));
		cat_input_button->node_buttons.push_back(std::move(camera_data_button));
		cat_input_button->node_buttons.push_back(std::move(fresnel_button));
		cat_input_button->node_buttons.push_back(std::move(geometry_button));
		cat_input_button->node_buttons.push_back(std::move(layer_weight_button));
		cat_input_button->node_buttons.push_back(std::move(light_path_button));
		cat_input_button->node_buttons.push_back(std::move(object_info_button));
		cat_input_button->node_buttons.push_back(std::move(rgb_button));
		cat_input_button->node_buttons.push_back(std::move(tangent_button));
		cat_input_button->node_buttons.push_back(std::move(tex_coord_button));
		cat_input_button->node_buttons.push_back(std::move(value_button));
		cat_input_button->node_buttons.push_back(std::move(wireframe_button));
	}

	// Shaders buttons
	{
		auto add_button = std::make_unique<GenericNodeButton<AddShaderNode>>();
		auto anisotropic_button = std::make_unique<GenericNodeButton<AnisotropicBSDFNode>>();
		auto diffuse_button = std::make_unique<GenericNodeButton<DiffuseBSDFNode>>();
		auto emission_button = std::make_unique<GenericNodeButton<EmissionNode>>();
		auto glass_button = std::make_unique<GenericNodeButton<GlassBSDFNode>>();
		auto glossy_button = std::make_unique<GenericNodeButton<GlossyBSDFNode>>();
		auto hair_button = std::make_unique<GenericNodeButton<HairBSDFNode>>();
		auto holdout_button = std::make_unique<GenericNodeButton<HoldoutNode>>();
		auto mix_button = std::make_unique<GenericNodeButton<MixShaderNode>>();
		auto principled_button = std::make_unique<GenericNodeButton<PrincipledBSDFNode>>();
		auto principled_hair_button = std::make_unique<GenericNodeButton<PrincipledHairNode>>();
		auto principled_volume_button = std::make_unique<GenericNodeButton<PrincipledVolumeNode>>();
		auto refraction_button = std::make_unique<GenericNodeButton<RefractionBSDFNode>>();
		auto sss_button = std::make_unique<GenericNodeButton<SubsurfaceScatteringNode>>();
		auto toon_button = std::make_unique<GenericNodeButton<ToonBSDFNode>>();
		auto translucent_button = std::make_unique<GenericNodeButton<TranslucentBSDFNode>>();
		auto transparent_button = std::make_unique<GenericNodeButton<TransparentBSDFNode>>();
		auto velvet_button = std::make_unique<GenericNodeButton<VelvetBSDFNode>>();
		auto vol_absorption_button = std::make_unique<GenericNodeButton<VolumeAbsorptionNode>>();
		auto vol_scatter_button = std::make_unique<GenericNodeButton<VolumeScatterNode>>();

		cat_shader_button->node_buttons.push_back(std::move(add_button));
		cat_shader_button->node_buttons.push_back(std::move(anisotropic_button));
		cat_shader_button->node_buttons.push_back(std::move(diffuse_button));
		cat_shader_button->node_buttons.push_back(std::move(emission_button));
		cat_shader_button->node_buttons.push_back(std::move(glass_button));
		cat_shader_button->node_buttons.push_back(std::move(glossy_button));
		cat_shader_button->node_buttons.push_back(std::move(hair_button));
		cat_shader_button->node_buttons.push_back(std::move(holdout_button));
		cat_shader_button->node_buttons.push_back(std::move(mix_button));
		cat_shader_button->node_buttons.push_back(std::move(principled_button));
		cat_shader_button->node_buttons.push_back(std::move(principled_hair_button));
		cat_shader_button->node_buttons.push_back(std::move(principled_volume_button));
		cat_shader_button->node_buttons.push_back(std::move(refraction_button));
		cat_shader_button->node_buttons.push_back(std::move(sss_button));
		cat_shader_button->node_buttons.push_back(std::move(toon_button));
		cat_shader_button->node_buttons.push_back(std::move(translucent_button));
		cat_shader_button->node_buttons.push_back(std::move(transparent_button));
		cat_shader_button->node_buttons.push_back(std::move(velvet_button));
		cat_shader_button->node_buttons.push_back(std::move(vol_absorption_button));
		cat_shader_button->node_buttons.push_back(std::move(vol_scatter_button));
	}

	// Texture buttons
	{
#ifdef INCLUDE_MAX_INTEGRATION
		auto max_tex_button = std::make_unique<GenericNodeButton<MaxTexmapShaderNode>>();
#endif
		auto brick_tex_button = std::make_unique<GenericNodeButton<BrickTextureNode>>();
		auto checker_tex_button = std::make_unique<GenericNodeButton<CheckerTextureNode>>();
		auto gradient_tex_button = std::make_unique<GenericNodeButton<GradientTextureNode>>();
		auto magic_tex_button = std::make_unique<GenericNodeButton<MagicTextureNode>>();
		auto musgrave_tex_button = std::make_unique<GenericNodeButton<MusgraveTextureNode>>();
		auto noise_tex_button = std::make_unique<GenericNodeButton<NoiseTextureNode>>();
		auto voronoi_tex_button = std::make_unique<GenericNodeButton<VoronoiTextureNode>>();
		auto wave_tex_button = std::make_unique<GenericNodeButton<WaveTextureNode>>();

#ifdef INCLUDE_MAX_INTEGRATION
		cat_texture_button->node_buttons.push_back(std::move(max_tex_button));
#endif
		cat_texture_button->node_buttons.push_back(std::move(brick_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(checker_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(gradient_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(magic_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(musgrave_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(noise_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(voronoi_tex_button));
		cat_texture_button->node_buttons.push_back(std::move(wave_tex_button));
	}

	// Color buttons
	{
		auto bright_contrast_button = std::make_unique<GenericNodeButton<BrightnessContrastNode>>();
		auto gamma_button = std::make_unique<GenericNodeButton<GammaNode>>();
		auto hsv_button = std::make_unique<GenericNodeButton<HSVNode>>();
		auto invert_button = std::make_unique<GenericNodeButton<InvertNode>>();
		auto light_falloff_button = std::make_unique<GenericNodeButton<LightFalloffNode>>();
		auto mix_rgb_button = std::make_unique<GenericNodeButton<MixRGBNode>>();
		auto rgb_curves_button = std::make_unique<GenericNodeButton<RGBCurvesNode>>();

		cat_color_button->node_buttons.push_back(std::move(bright_contrast_button));
		cat_color_button->node_buttons.push_back(std::move(gamma_button));
		cat_color_button->node_buttons.push_back(std::move(hsv_button));
		cat_color_button->node_buttons.push_back(std::move(invert_button));
		cat_color_button->node_buttons.push_back(std::move(light_falloff_button));
		cat_color_button->node_buttons.push_back(std::move(mix_rgb_button));
		cat_color_button->node_buttons.push_back(std::move(rgb_curves_button));
	}

	// Vector buttons
	{
		auto bump_button = std::make_unique<GenericNodeButton<BumpNode>>();
		auto displacement_button = std::make_unique<GenericNodeButton<DisplacementNode>>();
		auto normal_map_button = std::make_unique<GenericNodeButton<NormalMapNode>>();
		auto vector_transform_button = std::make_unique<GenericNodeButton<VectorTransformNode>>();

		cat_vector_button->node_buttons.push_back(std::move(bump_button));
		cat_vector_button->node_buttons.push_back(std::move(displacement_button));
		cat_vector_button->node_buttons.push_back(std::move(normal_map_button));
		cat_vector_button->node_buttons.push_back(std::move(vector_transform_button));
	}

	// Converter buttons
	{
		auto blackbody_button = std::make_unique<GenericNodeButton<BlackbodyNode>>();
		auto color_ramp_button = std::make_unique<GenericNodeButton<ColorRampNode>>();
		auto combine_hsv_button = std::make_unique<GenericNodeButton<CombineHSVNode>>();
		auto combine_rgb_button = std::make_unique<GenericNodeButton<CombineRGBNode>>();
		auto combine_xyz_button = std::make_unique<GenericNodeButton<CombineXYZNode>>();
		auto math_button = std::make_unique<GenericNodeButton<MathNode>>();
		auto rgb_to_bw_button = std::make_unique<GenericNodeButton<RGBToBWNode>>();
		auto separate_hsv_button = std::make_unique<GenericNodeButton<SeparateHSVNode>>();
		auto separate_rgb_button = std::make_unique<GenericNodeButton<SeparateRGBNode>>();
		auto separate_xyz_button = std::make_unique<GenericNodeButton<SeparateXYZNode>>();
		auto vector_math_button = std::make_unique<GenericNodeButton<VectorMathNode>>();
		auto wavelength_button = std::make_unique<GenericNodeButton<WavelengthNode>>();

		cat_converter_button->node_buttons.push_back(std::move(blackbody_button));
		cat_converter_button->node_buttons.push_back(std::move(color_ramp_button));
		cat_converter_button->node_buttons.push_back(std::move(combine_hsv_button));
		cat_converter_button->node_buttons.push_back(std::move(combine_rgb_button));
		cat_converter_button->node_buttons.push_back(std::move(combine_xyz_button));
		cat_converter_button->node_buttons.push_back(std::move(math_button));
		cat_converter_button->node_buttons.push_back(std::move(rgb_to_bw_button));
		cat_converter_button->node_buttons.push_back(std::move(separate_hsv_button));
		cat_converter_button->node_buttons.push_back(std::move(separate_rgb_button));
		cat_converter_button->node_buttons.push_back(std::move(separate_xyz_button));
		cat_converter_button->node_buttons.push_back(std::move(vector_math_button));
		cat_converter_button->node_buttons.push_back(std::move(wavelength_button));
	}

	// Add category buttons at the end because we need to std::move them into the list
	category_buttons.push_back(std::move(cat_input_button));
	category_buttons.push_back(std::move(cat_shader_button));
	category_buttons.push_back(std::move(cat_texture_button));
	category_buttons.push_back(std::move(cat_color_button));
	category_buttons.push_back(std::move(cat_vector_button));
	category_buttons.push_back(std::move(cat_converter_button));
}

void cse::NodeListSubwindow::handle_mouse_button(int button, int action, int /*mods*/) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (is_mouse_over_header()) {
			move_window_begin();
		}
		else if (is_category_button_under_mouse()) {
			select_category_button_under_mouse();
		}
		else if (is_creation_button_under_mouse()) {
			select_creation_button_under_mouse();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (subwindow_moving) {
			move_window_end();
		}
		else {
			reset_creation_buttons();
		}
	}
}

void cse::NodeListSubwindow::draw_content(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	// Node category buttons
	NodeCategoryButtonPlacer placer(Float2(0.0f, height_drawn), subwindow_width, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	for (const auto& category_button : category_buttons) {
		const Float2 button_position = placer.next_button_position();
		category_button->update_mouse_position(mouse_content_pos - button_position);
		category_button->draw(button_position, draw_context);
	}

	height_drawn += placer.get_draw_height();

	// Buttons
	for (const auto& node_button : get_active_creation_buttons()) {
		const Float2 button_location(0.0f, height_drawn);
		height_drawn += node_button->draw(draw_context, button_location, mouse_content_pos, subwindow_width);
	}

	content_height = height_drawn + UI_SUBWIN_NODE_LIST_BUTTON_VPADDING;
}

const std::list<std::unique_ptr<cse::NodeCreationButton>>& cse::NodeListSubwindow::get_active_creation_buttons() const
{
	for (const auto& this_category : category_buttons) {
		if (this_category->selected) {
			return this_category->node_buttons;
		}
	}
	return empty_creation_button_list;
}

bool cse::NodeListSubwindow::is_category_button_under_mouse() const
{
	for (const auto& this_category : category_buttons) {
		if (this_category->is_mouse_over_button()) {
			return true;
		}
	}
	return false;
}

bool cse::NodeListSubwindow::is_creation_button_under_mouse() const
{
	for (const auto& creation_button : get_active_creation_buttons()) {
		if (creation_button->is_mouse_over_button()) {
			return true;
		}
	}
	return false;
}

void cse::NodeListSubwindow::select_category_button_under_mouse()
{
	for (const auto& this_category : category_buttons) {
		this_category->selected = this_category->is_mouse_over_button();
	}
}

void cse::NodeListSubwindow::select_creation_button_under_mouse()
{
	if (const auto node_creation_helper = this->node_creation_helper.lock()) {
		for (const auto& creation_button : get_active_creation_buttons()) {
			if (creation_button->is_mouse_over_button()) {
				auto ptr = creation_button->create_node();
				node_creation_helper->set_node(ptr);
				creation_button->pressed = true;
				break;
			}
		}
	}
}

void cse::NodeListSubwindow::reset_creation_buttons()
{
	for (const auto& this_button : get_active_creation_buttons()) {
		this_button->pressed = false;
	}
	if (const auto node_creation_helper = this->node_creation_helper.lock()) {
		node_creation_helper->clear();
	}
}
