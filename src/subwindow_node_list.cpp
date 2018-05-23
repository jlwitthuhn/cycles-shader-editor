#include "subwindow_node_list.h"

#include <nanovg.h>
#include <GLFW/glfw3.h>

#include "config.h"

#include "button_category.h"
#include "buttons_nodes.h"
#include "gui_sizes.h"
#include "input_box.h"
#include "node_colors.h"
#include "node_converter.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_outputs.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"

CyclesShaderEditor::NodeListSubwindow::NodeListSubwindow(FloatPos screen_position) : NodeEditorSubwindow(screen_position, "Create Node")
{
	subwindow_width = UI_SUBWIN_NODE_LIST_WIDTH;

	// Category buttons -- cleaned up in ~NodeListSubwindow
	NodeCategoryButton* cat_input_button = new NodeCategoryButton(std::string("Input"));
	NodeCategoryButton* cat_shader_button = new NodeCategoryButton(std::string("Shader"));
	NodeCategoryButton* cat_texture_button = new NodeCategoryButton(std::string("Texture"));
	NodeCategoryButton* cat_color_button = new NodeCategoryButton(std::string("Color"));
	NodeCategoryButton* cat_vector_button = new NodeCategoryButton(std::string("Vector"));
	NodeCategoryButton* cat_converter_button = new NodeCategoryButton(std::string("Converter"));

	category_buttons.push_back(cat_input_button);
	category_buttons.push_back(cat_shader_button);
	category_buttons.push_back(cat_texture_button);
	category_buttons.push_back(cat_color_button);
	category_buttons.push_back(cat_vector_button);
	category_buttons.push_back(cat_converter_button);

	// Input buttons -- cleaned up in ~NodeCategoryButton
	NodeCreationButton* camera_data_button = new GenericNodeButton<CameraDataNode>();
	NodeCreationButton* fresnel_button = new GenericNodeButton<FresnelNode>();
	NodeCreationButton* geometry_button = new GenericNodeButton<GeometryNode>();
	NodeCreationButton* layer_weight_button = new GenericNodeButton<LayerWeightNode>();
	NodeCreationButton* light_path_button = new GenericNodeButton<LightPathNode>();
	NodeCreationButton* object_info_button = new GenericNodeButton<ObjectInfoNode>();
	NodeCreationButton* rgb_button = new GenericNodeButton<RGBNode>();
	NodeCreationButton* tangent_button = new GenericNodeButton<TangentNode>();
	NodeCreationButton* tex_coord_button = new GenericNodeButton<TextureCoordinateNode>();
	NodeCreationButton* value_button = new GenericNodeButton<ValueNode>();
	NodeCreationButton* wireframe_button = new GenericNodeButton<WireframeNode>();

	cat_input_button->node_buttons.push_back(camera_data_button);
	cat_input_button->node_buttons.push_back(fresnel_button);
	cat_input_button->node_buttons.push_back(geometry_button);
	cat_input_button->node_buttons.push_back(layer_weight_button);
	cat_input_button->node_buttons.push_back(light_path_button);
	cat_input_button->node_buttons.push_back(object_info_button);
	cat_input_button->node_buttons.push_back(rgb_button);
	cat_input_button->node_buttons.push_back(tangent_button);
	cat_input_button->node_buttons.push_back(tex_coord_button);
	cat_input_button->node_buttons.push_back(value_button);
	cat_input_button->node_buttons.push_back(wireframe_button);

	// Shaders buttons -- same ownership as input buttons
	NodeCreationButton* add_button = new GenericNodeButton<AddShaderNode>();
	NodeCreationButton* ao_button = new GenericNodeButton<AmbientOcculsionNode>();
	NodeCreationButton* anisotropic_button = new GenericNodeButton<AnisotropicBSDFNode>();
	NodeCreationButton* diffuse_button = new GenericNodeButton<DiffuseBSDFNode>();
	NodeCreationButton* emission_button = new GenericNodeButton<EmissionNode>();
	NodeCreationButton* glass_button = new GenericNodeButton<GlassBSDFNode>();
	NodeCreationButton* glossy_button = new GenericNodeButton<GlossyBSDFNode>();
	NodeCreationButton* hair_button = new GenericNodeButton<HairBSDFNode>();
	NodeCreationButton* holdout_button = new GenericNodeButton<HoldoutNode>();
	NodeCreationButton* mix_button = new GenericNodeButton<MixShaderNode>();
	NodeCreationButton* principled_button = new GenericNodeButton<PrincipledBSDFNode>();
	NodeCreationButton* refraction_button = new GenericNodeButton<RefractionBSDFNode>();
	NodeCreationButton* sss_button = new GenericNodeButton<SubsurfaceScatteringNode>();
	NodeCreationButton* toon_button = new GenericNodeButton<ToonBSDFNode>();
	NodeCreationButton* translucent_button = new GenericNodeButton<TranslucentBSDFNode>();
	NodeCreationButton* transparent_button = new GenericNodeButton<TransparentBSDFNode>();
	NodeCreationButton* velvet_button = new GenericNodeButton<VelvetBSDFNode>();
	NodeCreationButton* vol_absorption_button = new GenericNodeButton<VolumeAbsorptionNode>();
	NodeCreationButton* vol_scatter_button = new GenericNodeButton<VolumeScatterNode>();

	cat_shader_button->node_buttons.push_back(add_button);
	cat_shader_button->node_buttons.push_back(ao_button);
	cat_shader_button->node_buttons.push_back(anisotropic_button);
	cat_shader_button->node_buttons.push_back(diffuse_button);
	cat_shader_button->node_buttons.push_back(emission_button);
	cat_shader_button->node_buttons.push_back(glass_button);
	cat_shader_button->node_buttons.push_back(glossy_button);
	cat_shader_button->node_buttons.push_back(hair_button);
	cat_shader_button->node_buttons.push_back(holdout_button);
	cat_shader_button->node_buttons.push_back(mix_button);
	cat_shader_button->node_buttons.push_back(principled_button);
	cat_shader_button->node_buttons.push_back(refraction_button);
	cat_shader_button->node_buttons.push_back(sss_button);
	cat_shader_button->node_buttons.push_back(toon_button);
	cat_shader_button->node_buttons.push_back(translucent_button);
	cat_shader_button->node_buttons.push_back(transparent_button);
	cat_shader_button->node_buttons.push_back(velvet_button);
	cat_shader_button->node_buttons.push_back(vol_absorption_button);
	cat_shader_button->node_buttons.push_back(vol_scatter_button);

	// Texture buttons -- same ownership as input buttons
#ifdef INCLUDE_MAX_INTEGRATION
	NodeCreationButton* max_tex_button = new GenericNodeButton<MaxTexmapShaderNode>();
#endif
	NodeCreationButton* brick_tex_button = new GenericNodeButton<BrickTextureNode>();
	NodeCreationButton* checker_tex_button = new GenericNodeButton<CheckerTextureNode>();
	NodeCreationButton* gradient_tex_button = new GenericNodeButton<GradientTextureNode>();
	NodeCreationButton* magic_tex_button = new GenericNodeButton<MagicTextureNode>();
	NodeCreationButton* musgrave_tex_button = new GenericNodeButton<MusgraveTextureNode>();
	NodeCreationButton* noise_tex_button = new GenericNodeButton<NoiseTextureNode>();
	NodeCreationButton* voronoi_tex_button = new GenericNodeButton<VoronoiTextureNode>();
	NodeCreationButton* wave_tex_button = new GenericNodeButton<WaveTextureNode>();

#ifdef INCLUDE_MAX_INTEGRATION
	cat_texture_button->node_buttons.push_back(max_tex_button);
#endif
	cat_texture_button->node_buttons.push_back(brick_tex_button);
	cat_texture_button->node_buttons.push_back(checker_tex_button);
	cat_texture_button->node_buttons.push_back(gradient_tex_button);
	cat_texture_button->node_buttons.push_back(magic_tex_button);
	cat_texture_button->node_buttons.push_back(musgrave_tex_button);
	cat_texture_button->node_buttons.push_back(noise_tex_button);
	cat_texture_button->node_buttons.push_back(voronoi_tex_button);
	cat_texture_button->node_buttons.push_back(wave_tex_button);

	// Color buttons -- same ownership as input buttons
	NodeCreationButton* bright_contrast_button = new GenericNodeButton<BrightnessContrastNode>();
	NodeCreationButton* gamma_button = new GenericNodeButton<GammaNode>();
	NodeCreationButton* hsv_button = new GenericNodeButton<HSVNode>();
	NodeCreationButton* invert_button = new GenericNodeButton<InvertNode>();
	NodeCreationButton* light_falloff_button = new GenericNodeButton<LightFalloffNode>();
	NodeCreationButton* mix_rgb_button = new GenericNodeButton<MixRGBNode>();
	NodeCreationButton* rgb_curves_button = new GenericNodeButton<RGBCurvesNode>();

	cat_color_button->node_buttons.push_back(bright_contrast_button);
	cat_color_button->node_buttons.push_back(gamma_button);
	cat_color_button->node_buttons.push_back(hsv_button);
	cat_color_button->node_buttons.push_back(invert_button);
	cat_color_button->node_buttons.push_back(light_falloff_button);
	cat_color_button->node_buttons.push_back(mix_rgb_button);
	cat_color_button->node_buttons.push_back(rgb_curves_button);

	// Vector buttons -- same ownership as input buttons
	NodeCreationButton* bump_button = new GenericNodeButton<BumpNode>();
	NodeCreationButton* normal_map_button = new GenericNodeButton<NormalMapNode>();
	NodeCreationButton* vector_transform_button = new GenericNodeButton<VectorTransformNode>();

	cat_vector_button->node_buttons.push_back(bump_button);
	cat_vector_button->node_buttons.push_back(normal_map_button);
	cat_vector_button->node_buttons.push_back(vector_transform_button);

	// Converter buttons
	NodeCreationButton* blackbody_button = new GenericNodeButton<BlackbodyNode>();
	NodeCreationButton* combine_hsv_button = new GenericNodeButton<CombineHSVNode>();
	NodeCreationButton* combine_rgb_button = new GenericNodeButton<CombineRGBNode>();
	NodeCreationButton* combine_xyz_button = new GenericNodeButton<CombineXYZNode>();
	NodeCreationButton* math_button = new GenericNodeButton<MathNode>();
	NodeCreationButton* rgb_to_bw_button = new GenericNodeButton<RGBToBWNode>();
	NodeCreationButton* separate_hsv_button = new GenericNodeButton<SeparateHSVNode>();
	NodeCreationButton* separate_rgb_button = new GenericNodeButton<SeparateRGBNode>();
	NodeCreationButton* separate_xyz_button = new GenericNodeButton<SeparateXYZNode>();
	NodeCreationButton* vector_math_button = new GenericNodeButton<VectorMathNode>();
	NodeCreationButton* wavelength_button = new GenericNodeButton<WavelengthNode>();

	cat_converter_button->node_buttons.push_back(blackbody_button);
	cat_converter_button->node_buttons.push_back(combine_hsv_button);
	cat_converter_button->node_buttons.push_back(combine_rgb_button);
	cat_converter_button->node_buttons.push_back(combine_xyz_button);
	cat_converter_button->node_buttons.push_back(math_button);
	cat_converter_button->node_buttons.push_back(rgb_to_bw_button);
	cat_converter_button->node_buttons.push_back(separate_hsv_button);
	cat_converter_button->node_buttons.push_back(separate_rgb_button);
	cat_converter_button->node_buttons.push_back(separate_xyz_button);
	cat_converter_button->node_buttons.push_back(vector_math_button);
	cat_converter_button->node_buttons.push_back(wavelength_button);

	active_button = nullptr;

	active_category = cat_shader_button;
}

CyclesShaderEditor::NodeListSubwindow::~NodeListSubwindow()
{
	for (NodeCategoryButton* category_button : category_buttons) {
		delete category_button;
	}
}

void CyclesShaderEditor::NodeListSubwindow::pre_draw()
{
	for (NodeCategoryButton* category_button : category_buttons) {
		if (category_button == active_category) {
			category_button->pressed = true;
		}
		else {
			category_button->pressed = false;
		}
	}
}

void CyclesShaderEditor::NodeListSubwindow::handle_mouse_button(int button, int action, int /*mods*/) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (is_mouse_over_header()) {
			move_window_begin();
		}
		else if (get_category_button_under_mouse() != nullptr) {
			active_category = get_category_button_under_mouse();
		}
		else if (get_button_under_mouse() != nullptr) {
			if (active_button != nullptr) {
				active_button->pressed = false;
				active_button = nullptr;
			}
			NodeCreationButton* node_button = get_button_under_mouse();
			node_button->pressed = true;
			active_button = node_button;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (subwindow_moving) {
			move_window_end();
		}
		else {
			if (active_button != nullptr) {
				active_button->pressed = false;
				active_button = nullptr;
			}
		}
	}
}

CyclesShaderEditor::NodeCategoryButton* CyclesShaderEditor::NodeListSubwindow::get_category_button_under_mouse()
{
	for (NodeCategoryButton* category_button : category_buttons) {
		if (category_button->is_mouse_over_button()) {
			return category_button;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeCreationButton* CyclesShaderEditor::NodeListSubwindow::get_button_under_mouse()
{
	if (active_category == nullptr) {
		return nullptr;
	}

	for (NodeCreationButton* node_button : active_category->node_buttons) {
		if (node_button->is_mouse_over_button()) {
			return node_button;
		}
	}

	return nullptr;
}

void CyclesShaderEditor::NodeListSubwindow::draw_content(NVGcontext* draw_context)
{
	float height_drawn = 0.0f;

	// Node category buttons
	NodeCategoryButtonPlacer placer(FloatPos(0.0f, height_drawn), subwindow_width, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	for (NodeCategoryButton* category_button : category_buttons) {
		FloatPos button_position = placer.next_button_position();
		category_button->update_mouse_position(mouse_panel_pos - button_position);
		category_button->draw(button_position, draw_context);
	}

	height_drawn += placer.get_draw_height();

	// Buttons
	if (active_category != nullptr) {
		for (NodeCreationButton* node_button : active_category->node_buttons) {
			FloatPos button_location(0.0f, height_drawn);
			height_drawn += node_button->draw(draw_context, button_location, mouse_panel_pos, subwindow_width);
		}
	}

	content_height = height_drawn + UI_SUBWIN_NODE_LIST_BUTTON_VPADDING;
}
